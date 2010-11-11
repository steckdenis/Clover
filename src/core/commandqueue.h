#ifndef COAL_COMMANDQUEUE_H
#define COAL_COMMANDQUEUE_H

namespace Coal {

   class CommandQueue
   {
   public:
      CommandQueue();
      ~CommandQueue();
   };
}

struct _cl_command_queue : public Coal::CommandQueue
{
};


#endif
