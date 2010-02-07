#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <execinfo.h>

/* get REG_EIP from ucontext.h */
#ifndef __USE_GNU
#define __USE_GNU
#endif
#include <ucontext.h>
#ifndef EIP
#define EIP     14
#endif

#if (defined (__x86_64__))
#ifndef REG_RIP
#define REG_RIP REG_INDEX(rip) /* seems to be 16 */
#endif
#endif

static void bt_sighandler(int sig, siginfo_t *info, void *secret)
{
   void *trace[16];
   char **messages = (char **)NULL;
   int i, trace_size = 0;
   ucontext_t *uc = (ucontext_t *)secret;

   void *pnt = NULL;
#if defined(__x86_64__)
   pnt = (void*) uc->uc_mcontext.gregs[REG_RIP] ;
#elif defined(__i386__)
   pnt = (void*) uc->uc_mcontext.gregs[REG_EIP] ;
#endif

   /* Do something useful with siginfo_t */
   if (sig == SIGSEGV)
      printf("Got signal %d, faulty address is %p, "
            "from %p\n", sig, info->si_addr, pnt); 
   else
      printf("Got signal %d\n", sig);

   trace_size = backtrace(trace, 16);
   /* overwrite sigaction with caller's address */
   trace[1] = pnt;

   messages = backtrace_symbols(trace, trace_size);
   /* skip first stack frame (points here) */
   printf("[bt] Execution path:\n");
   for (i=1; i<trace_size; ++i)
      printf("[bt] %s\n", messages[i]);

   exit(0);
}

void install_bt_handler(void)
{
   struct sigaction sa;

   sa.sa_sigaction = bt_sighandler;
   sigemptyset (&sa.sa_mask);
   sa.sa_flags = SA_RESTART | SA_SIGINFO;

   sigaction(SIGSEGV, &sa, NULL);
   sigaction(SIGUSR1, &sa, NULL);
}

#if 0
int foo()
{
   char *p = (char *)0xdeadbeef;
   *p = 10;   /* CRASH here!! */
}

int bar()
{
   foo();
}

int main()
{
   install_bt_handler();
   bar();
}
#endif
