#ifndef		__TIMER_HEADER_
#define 	__TIMER_HEADER_
        
	void	PWM_Init( void );	//初始化PWM输出
        void   Capture_Init( void );   //初始化输入捕获
        extern unsigned int period;
        extern unsigned int pos_time;
        extern char flag;
#endif