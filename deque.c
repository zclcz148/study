#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

//函数标识,用于在核心逻辑处理函数中指示操作
#define IS_PUSHFRONT 0
#define IS_PUSHBACK 1
#define IS_POPFRONT 2
#define IS_POPBACK 3
#define IS_FRONT 4
#define IS_BACK 5
#define IS_START 6
#define IS_END 7
#define IS_IS_EMPTY 8
#define IS_MAKEEMPTY 9
#define ERR_CLAEAR_OPTION 10

//其他标识变量
#define head_order false
#define tail_order true
#define DEQUE_ERROR 0xFFFF

//队列元素结构定义,该队列使用双向链表实现
struct __DEQUE_NODE_STRUCT__
{
  struct __DEQUE_NODE_STRUCT__ *last;
  struct __DEQUE_NODE_STRUCT__ *next;
  int v;
};

#define iterator struct __DEQUE_NODE_STRUCT__* //封装的迭代器
#define __DEQUE_NODE__ struct __DEQUE_NODE_STRUCT__ //封装的队列节点类型,不应当私自使用

//封装的队列类,用函数指针模拟成员函数,使用 队列变量.函数名() 调用
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

//更新队列的核心逻辑函数,处理各个方法函数的逻辑,不应当私自调用
static iterator UpdateDeque(iterator node,int opera_func)
{
  static iterator ufront = NULL;
  static iterator uback = NULL;
  static iterator start_marker = NULL; //Start函数实现方法的标识变量
  static iterator end_marker = NULL; //End函数实现方法的标识变量
  iterator tmp;
  int guard = 0; //内存错误处理标识变量
  if( (ufront == NULL && uback == NULL) //如果压入队列时队列为空则初始化
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
      else if(opera_func == IS_PUSHFRONT || opera_func == IS_PUSHBACK) //空指针和内存错误处理
        {
          printf("\nOut of memory or heap memory application failed!\n");
          guard = -1;
          goto err_handle;
        }
      if(start_marker != NULL || end_marker != NULL) //重置Start和End标识变量
        {
          start_marker = end_marker = NULL;
          ufront->last = uback->next = NULL;
        }
      switch(opera_func) //处理方法函数的逻辑选择
        {
        case ERR_CLAEAR_OPTION: //错误处理逻辑,回收资源并清理
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
        //以下都是正常成员函数的处理逻辑,使用 "IS_函数名" 标识
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

        case IS_FRONT:
          return ufront;

        case IS_BACK:
          return uback;

        case IS_IS_EMPTY:
          if(ufront == NULL && uback == NULL) return NULL;
          return (iterator)DEQUE_ERROR;

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
          if(ufront == NULL) return (iterator)DEQUE_ERROR;
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
            return (iterator) DEQUE_ERROR;
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

        default: //假如私自调用传入未知选项的处理
          printf("\nUnknown Operation!\n");
          exit(1);
        }
    }
  return NULL; //显式返回
}

//将传入的值压入队首,不返回值
void PushFront(int var)
{
  iterator t = (iterator)malloc(sizeof(__DEQUE_NODE__));
  t->last = t->next = NULL;
  t->v = var;
  UpdateDeque(t,IS_PUSHFRONT);
}

//将传入的值压入队尾,不返回值
void PushBack(int var)
{
  iterator t = (iterator)malloc(sizeof(__DEQUE_NODE__));
  t->last = t->next = NULL;
  t->v = var;
  UpdateDeque(t,IS_PUSHBACK);
}

//弹出队首元素,不返回值
void PopFront(void)
{
  if(UpdateDeque(NULL,IS_POPFRONT) != NULL)
    {
      printf("\nTrying to pop an empty deque!\n");
      exit(1);
    }
}

//弹出队尾元素,不返回值
void PopBack(void)
{
  if(UpdateDeque(NULL,IS_POPBACK) != NULL)
    {
      printf("\nTrying to pop an empty deque!\n");
      exit(1);
    }
}

//清空队列
void MakeEmpty(void)
{
  UpdateDeque(NULL,IS_MAKEEMPTY);
}

//提供了从队首或队尾打印队列的两种方式的函数,
//传入的参数为head_order则从队首打印,tail_order则从队尾打印
void PrintDeque(bool h_l)
{
  iterator q = NULL;
  if(h_l != head_order && h_l != tail_order)
    {
      printf("\nUnknown print order!\n");
      UpdateDeque(NULL,ERR_CLAEAR_OPTION);
    }
  if(h_l)
    {
      q = UpdateDeque(NULL,IS_BACK);
      while(q != NULL)
        {
          printf("%d ",q->v);
          q = q->last;
        }
      printf("\n");
    }
  else
    {
      q = UpdateDeque(NULL,IS_FRONT);
      while(q != NULL)
        {
          printf("%d ",q->v);
          q = q->next;
        }
      printf("\n");
    }
}

//返回指示队列是否为空的值,空为true,非空为false
bool IsEmpty(void)
{
  return UpdateDeque(NULL,IS_IS_EMPTY) == NULL ? true : false;
}

//返回指向队首元素的迭代器
iterator Front(void)
{
  return UpdateDeque(NULL,IS_FRONT);
}

//返回指向队尾元素的迭代器
iterator Back(void)
{
  return UpdateDeque(NULL,IS_BACK);
}

//返回指向队首元素前一个元素位置的指针,该指针只可用于与迭代器比较
iterator Start(void)
{
  return UpdateDeque(NULL,IS_START);
}

//返回指向队尾元素后一个元素位置的指针,该指针只可用于与迭代器比较
iterator End(void)
{
  return UpdateDeque(NULL,IS_END);
}

//用于初始化队列各方法函数的函数
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


//小小地封装一下
struct __DEQUE_TYPE__ Deque(struct __DEQUE_TYPE__ *p)
{
  InitDeque(p);
  return (*p);
}

int main(void)
{
  deque d;
  d = Deque(&d); //初始化队列

  for(int i = 1; i<11; ++i)
    d.push_front(i);

  for(int i = 1; i<11; ++i)
    d.push_back(i);

  d.print_deque(tail_order); //从队尾打印
  d.pop_back();
  d.print_deque(tail_order);

  printf("%s",d.is_empty() ? "The deque is empty\n" : "The deque is not empty\n");
  d.push_back(123);
  d.pop_front();

  for(auto p = d.front(); p != d.end(); p = p->next) //正向迭代器
    printf("%d ",p->v);
  printf("\n");

  for(auto p = d.back(); p != d.start(); p = p->last) //反向迭代器
    printf("%d ",p->v);
  printf("\n");

  d.make_empty();
  printf("%s",d.is_empty() ? "The deque is empty\n" : "The deque is not empty\n");
  return 0;
}
/* 2026-4-4 */
