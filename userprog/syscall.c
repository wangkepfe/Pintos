#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"
#include "filesys/file.h"
#include "filesys/filesys.h"
#include "userprog/pagedir.h"
#include "userprog/process.h"
#include "threads/malloc.h"

///////////////////////////////////////////////////////////////////////////////////////
//declarations

//original
void syscall_init (void);
static void syscall_handler (struct intr_frame *);

//lab1
void sys_halt (void);
bool sys_create (const char *file, unsigned initial_size);
int sys_open (const char *file);
void sys_close (int fd);
int sys_read (int fd, void *buffer, unsigned size);
int sys_write (int fd, const void *buffer, unsigned size);

//lab3
int sys_exec(const char* cmd_line);
void sys_exit(int status);
int sys_wait(int pid);

//lab4
void sys_seek(int fd, unsigned position);
unsigned sys_tell(int fd);
int sys_filesize(int fd);
bool sys_remove(const char* file_name);

//structure
struct process_file {
  struct file *file;
  int fd;
  struct list_elem elem;
};

//file handle
int process_add_file (struct file *f);
struct file* process_get_file (int fd);
void process_close_file (int fd);

//lab3C
bool pointerValid(void *p);
bool stringValid(const char *p);
bool bufferValid(void *p, unsigned size);

///////////////////////////////////////////////////////////////////////////////////////
//implemetations

//original

void syscall_init (void){
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void syscall_handler (struct intr_frame *f){
  int *p = f->esp;

  if(!pointerValid(p))return;
  switch (*p) {
    //3 parameters
    case SYS_READ:
    case SYS_WRITE: if(!pointerValid(p + 3))return;
    //2 parameters
    case SYS_SEEK:
    case SYS_CREATE: if(!pointerValid(p + 2))return;
    //1 parameter
    case SYS_TELL:
    case SYS_FILESIZE:
    case SYS_REMOVE:
    case SYS_OPEN:
    case SYS_EXEC:
    case SYS_EXIT:
    case SYS_WAIT: if(!pointerValid(p + 1))return;
    //0 parameter
    case SYS_CLOSE:
    case SYS_HALT:
    default:;
  }

  switch (*p) {

    //lab1
    case SYS_HALT: sys_halt(); break;
    case SYS_CREATE:f->eax = sys_create((const char*) *(p+1),(unsigned) *(p+2));break;
    case SYS_OPEN:f->eax = sys_open((const char*) *(p+1));break;
    case SYS_CLOSE: sys_close((const char*)*(p+1)); break;
    case SYS_READ:f->eax = sys_read((int) *(p+1),(void*) *(p+2),(unsigned) *(p+3));break;
    case SYS_WRITE:f->eax = sys_write((int) *(p+1),(void*) *(p+2),(unsigned) *(p+3));break;

    //lab3
    case SYS_EXEC: f->eax = sys_exec((const char*) *(p+1)); break;
    case SYS_WAIT: f->eax = sys_wait((int) *(p+1)); break;
    case SYS_EXIT: sys_exit((int) *(p+1)); break;

    //lab4
    case SYS_SEEK: sys_seek((int) *(p+1), (unsigned) *(p+2)); break;
    case SYS_TELL: f->eax = sys_tell((int) *(p+1)); break;
    case SYS_FILESIZE: f->eax = sys_filesize((int) *(p+1)); break;
    case SYS_REMOVE: f->eax = sys_remove((const char*) *(p+1)); break;

    default:;
  }
}

//lab1

void sys_halt (void) {
  power_off();
}

bool sys_create (const char *file, unsigned initial_size){
  if(!stringValid(file))
    return false;
  return filesys_create(file, initial_size);
}

int fd_count = 2;

int sys_open (const char *file) {
  if(!stringValid(file))
    return false;
  struct file *f = filesys_open(file);
  if(f == NULL)
    return -1;
  int fd = process_add_file(f);
  return fd;
}

void sys_close (int fd) {
  process_close_file(fd);
}

int sys_read (int fd, void *buffer, unsigned size) {
  if(!bufferValid(buffer, size))
    return false;
  if (fd == STDIN_FILENO){
      unsigned i;
      char* local_buffer = (char *) buffer;
      for (i = 0; i < size; i++)
	        local_buffer[i] = input_getc();
      return size;
    }
  struct file *f = process_get_file(fd);
  if(f == NULL)
    return -1;
  int bytes = file_read(f, buffer, size);
  return bytes;
}

int sys_write (int fd, const void *buffer, unsigned size) {
  if(!bufferValid(buffer, size)){
    return false;
  }
  if (fd == STDOUT_FILENO){
    putbuf(buffer, size);
    return size;
  }
  struct file *f = process_get_file(fd);
  if(f == NULL){
    //printf("can't get file by fd!\n");
    return -1;
  }
  int bytes = file_write(f, buffer, size);
  //printf("sys_write return %d\n",bytes);
  return bytes;
}
//lab3

void sys_exit (int status) {
  process_close_all_file ();//sys_exit close all open files

  thread_current()->myParent_child->status = status;
  sema_up(&thread_current()->myParent_child->semaForWait);
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit();
}

int sys_exec(const char* cmd_line){
  if(!stringValid(cmd_line))
    return -1;
  return process_execute(cmd_line);
}

int sys_wait(int pid){
  return process_wait(pid);
}

//lab4
void sys_seek(int fd, unsigned position){
  struct file *f = process_get_file(fd);
  if(f == NULL)
    return;
  file_seek(f, position);
}

unsigned sys_tell(int fd){
  struct file *f = process_get_file(fd);
  if(f == NULL)
    return -1;
  return file_tell(f);
}

int sys_filesize(int fd){
  struct file *f = process_get_file(fd);
  if(f == NULL)
    return -1;
  return file_length(f);
}

bool sys_remove(const char* file_name){
  if(!stringValid(file_name))
    return false;
  return filesys_remove(file_name);
}

//file handle

int process_add_file (struct file *f){
  struct process_file *pf = malloc(sizeof(struct process_file));
  pf->file = f;
  pf->fd = thread_current()->fd;
  thread_current()->fd++;
  list_push_back(&thread_current()->file_list, &pf->elem);
  return pf->fd;
}

struct file* process_get_file (int fd){
  struct thread *t = thread_current();
  struct list_elem *e;

  for (e = list_begin (&t->file_list);  e != list_end (&t->file_list); e = list_next (e)){
      struct process_file *pf = list_entry (e, struct process_file, elem);
      if (fd == pf->fd){
	        return pf->file;
	    }
  }
  return NULL;
}

//in thread exit need to close all the open file

void process_close_file (int fd){
  struct thread *t = thread_current();
  struct list_elem *next, *e = list_begin(&t->file_list);
  while (e != list_end (&t->file_list)){
      next = list_next(e);
      struct process_file *pf = list_entry (e, struct process_file, elem);
      if (fd == pf->fd){
	        file_close(pf->file);
	        list_remove(&pf->elem);
	        free(pf);
          break;
	     }
      e = next;
  }
}

void process_close_all_file (){
  struct thread *t = thread_current();
  struct list_elem *next, *e = list_begin(&t->file_list);
  while (e != list_end (&t->file_list)){
      next = list_next(e);
      struct process_file *pf = list_entry (e, struct process_file, elem);
      file_close(pf->file);
      list_remove(&pf->elem);
      free(pf);
      e = next;
  }
}

//lab3C
bool pointerValid(void *p){
  //check if the pointer is NULL
  if(p != NULL && is_user_vaddr(p) && pagedir_get_page(thread_current()->pagedir, p) != NULL)
    return true;
  else
  {
    //printf("Pointer invalid detected!\n");
    sys_exit(-1);
  }
  return false;
}

bool stringValid(const char *p){
  int i;
  for(i = 0; i < 50; i++){//while loop
    if(pointerValid((int*)p + i)){
      if(p + i == NULL)
      {
        sys_exit(-1);
        return false;
      }
      if(p[i] == '\0')
        return true;
    }
  }
  return false;
}

bool bufferValid(void *p, unsigned size){
  //printf("buffer validation: buffer is %s,size is %d\n",p,size)
  unsigned i;
  for(i = 0; i < size; i++){
    if(!pointerValid(p))
      return false;
  }
  return true;
}
