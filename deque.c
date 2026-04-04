#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#define IS_PUSHFRONT 0
#define IS_PUSHBACK 1
#define IS_POPFRONT 2
#define IS_POPBACK 3
#define IS_RETURNFRONT 4
#define IS_RETURNBACK 5
#define IS_START 6
#define IS_END 7
#define IS_EMPTY 8
#define IS_MAKEEMPTY 9
#define ERR_CLAEAR_OPTION 10
#define up_order true
#define down_order false

struct __DEQUE_NODE_STRUCT__
{
  struct __DEQUE_NODE_STRUCT__ *last;
  struct __DEQUE_NODE_STRUCT__ *next;
  int v;
};

#define iterator struct __DEQUE_NODE_STRUCT__*
#define __DEQUE_NODE__ struct __DEQUE_NODE_STRUCT__

typedef struct __DEQUE_TYPE__
{
  bool (*is_empty)(void);
  iterator (*front)(void);
  iterator (*back)(void);
  iterator (*start)(void);
  iterator (*end)(void);
  void (*push_front)(int);
  void (*push_back)(int);
  void (*pop_front)(void);
  void (*pop_back)(void);
  void (*print_deque)(bool);
  void (*make_empty)(void);
} deque;

static iterator UpdateDeque(iterator node,int opera_func)
{
  static iterator ufront = NULL;
  static iterator uback = NULL;
  static iterator start_marker = NULL;
  static iterator end_marker = NULL;
  iterator tmp;
  int guard = 0;
  if( (ufront == NULL && uback == NULL)
      &&
      (opera_func == IS_PUSHFRONT || opera_func == IS_PUSHBACK) )
    {
      ufront = uback = node;
      return NULL;
    }
  else
    {
      if(node != NULL)
        {
          tmp = node;
        }
      else if(opera_func == IS_PUSHFRONT || opera_func == IS_PUSHBACK)
        {
          printf("\nOut of memory or heap memory application failed!\n");
          guard = -1;
          goto err_handle;
        }
      if(start_marker != NULL || end_marker != NULL)
        {
          start_marker = end_marker = NULL;
          ufront->last = uback->next = NULL;
        }
      switch(opera_func)
        {
        case ERR_CLAEAR_OPTION:
err_handle:
          if(guard == -1) { guard = 0; }
          if(ufront == NULL && uback == NULL) exit(1);
          uback = ufront->next;
          while(ufront != NULL)
            {
              if(uback == NULL) {free(ufront); break;}
              free(ufront);
              ufront = uback; uback = uback->next;
            }
          exit(1);
        case IS_PUSHFRONT:
          tmp->next = ufront;
          ufront->last = tmp;
          ufront = tmp;
          return NULL;
        case IS_PUSHBACK:
          tmp->last = uback;
          uback->next = tmp;
          uback = tmp;
          return NULL;
        case IS_RETURNFRONT:
          return ufront;
        case IS_RETURNBACK:
          return uback;
        case IS_EMPTY:
          if(ufront == NULL && uback == NULL) return NULL;
          return (iterator)0xFFFF;
        case IS_MAKEEMPTY:
          if(ufront == NULL && uback == NULL)
            {
              printf("\nTrying to clear an empty deque!\n");
              exit(1);
            }
          uback = ufront->next;
          while(ufront != NULL)
            {
              if(uback == NULL) {free(ufront); break;}
              free(ufront);
              ufront = uback; uback = uback->next;
            }
          ufront = uback = NULL;
          return NULL;
        case IS_POPFRONT:
          if(ufront == NULL) return (iterator)0xFFFF;
          ufront = ufront->next;
          if(ufront == NULL)
            {
              ufront = uback = NULL;
              return NULL;
            }
          free(ufront->last);
          ufront->last = NULL;
          return NULL;
        case IS_POPBACK:
          if(uback == NULL)
            return (iterator) 0xFFFF;
          uback = uback->last;
          if(uback == NULL)
            {
              uback = ufront = NULL;
              return NULL;
            }
          free(uback->next);
          uback->next = NULL;
          return NULL;
        case IS_START:
          if(ufront == NULL)
            {
              printf("\nCannot create iterator with an empty deque!\n");
              exit(1);
            }
          start_marker = (iterator)0x01;
          ufront->last = start_marker;
          return start_marker;
        case IS_END:
          if(uback == NULL)
            {
              printf("\nCannot create iterator with an empty deque!\n");
              exit(1);
            }
          end_marker = (iterator)0x02;
          uback->next = end_marker;
          return end_marker;
        default:
          printf("\nUnknown Operation!\n");
          exit(1);
        }
    }
  return NULL;
}

void PushFront(int var)
{
  iterator t = (iterator)malloc(sizeof(__DEQUE_NODE__));
  t->last = t->next = NULL;
  t->v = var;
  UpdateDeque(t,IS_PUSHFRONT);
}

void PushBack(int var)
{
  iterator t = (iterator)malloc(sizeof(__DEQUE_NODE__));
  t->last = t->next = NULL;
  t->v = var;
  UpdateDeque(t,IS_PUSHBACK);
}

void PopFront(void)
{
  if(UpdateDeque(NULL,IS_POPFRONT) != NULL)
    {
      printf("\nTrying to pop an empty deque!\n");
      exit(1);
    }
}

void PopBack(void)
{
  if(UpdateDeque(NULL,IS_POPBACK) != NULL)
    {
      printf("\nTrying to pop an empty deque!\n");
      exit(1);
    }
}

void MakeEmpty(void)
{
  UpdateDeque(NULL,IS_MAKEEMPTY);
}

void PrintDeque(bool h_l)
{
  iterator q = NULL;
  if(h_l != up_order && h_l != down_order)
    {
      printf("\nUnknown print order!\n");
      UpdateDeque(NULL,ERR_CLAEAR_OPTION);
    }
  if(h_l)
    {
      q = UpdateDeque(NULL,IS_RETURNBACK);
      while(q != NULL)
        {
          printf("%d ",q->v);
          q = q->last;
        }
      printf("\n");
    }
  else
    {
      q = UpdateDeque(NULL,IS_RETURNFRONT);
      while(q != NULL)
        {
          printf("%d ",q->v);
          q = q->next;
        }
      printf("\n");
    }
}

bool IsEmpty(void)
{
  return UpdateDeque(NULL,IS_EMPTY) == NULL ? true : false;
}

iterator Front(void)
{
  return UpdateDeque(NULL,IS_RETURNFRONT);
}

iterator Back(void)
{
  return UpdateDeque(NULL,IS_PUSHBACK);
}

iterator Start(void)
{
  return UpdateDeque(NULL,IS_START);
}

iterator End(void)
{
  return UpdateDeque(NULL,IS_END);
}

void InitDeque(struct __DEQUE_TYPE__ *p)
{
  p->is_empty = IsEmpty;
  p->front = Front;
  p->back = Back;
  p->start = Start;
  p->end = End;
  p->push_front = PushFront;
  p->push_back = PushBack;
  p->pop_front = PopFront;
  p->pop_back = PopBack;
  p->print_deque = PrintDeque;
  p->make_empty = MakeEmpty;
}

struct __DEQUE_TYPE__ Deque(struct __DEQUE_TYPE__ *p)
{
  InitDeque(p);
  return (*p);
}

int main(void)
{
  deque d;
  d = Deque(&d);
  for(int i = 1; i<11; ++i)
    {
      d.push_front(i);
    }
  for(int i = 1; i<11; ++i)
    {
      d.push_back(i);
    }
  d.print_deque(down_order);
  d.pop_back(); d.pop_front();
  d.print_deque(down_order);
  printf("%s",d.is_empty() ? "The deque is empty\n" : "The deque is not empty\n");
  d.push_back(123);
  d.pop_front();
  for(auto p = d.front(); p != d.end(); p = p->next)
    {
      printf("%d ",p->v);
    }
  printf("\n");
  d.make_empty();
  printf("%s",d.is_empty() ? "The deque is empty\n" : "The deque is not empty\n");
  return 0;
}
