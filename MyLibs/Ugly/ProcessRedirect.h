
#include <boost/function.hpp>

typedef boost::function<void (const char*, size_t)> OutputHandlerFn;

void run_redirected_process (const char* commandLine, OutputHandlerFn outputHandler);
