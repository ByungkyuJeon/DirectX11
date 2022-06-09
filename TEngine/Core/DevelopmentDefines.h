#pragma once

#if DEBUG
#define TO_STR(msg) #msg
#define FORMAT(line) __FILE__ "(" TO_STR(line) "): TODO - "
#define TODO FORMAT(__LINE__)
#else
#define TODO 
#endif