#ifndef		__TIMER_HEADER_
#define 	__TIMER_HEADER_
        
	void	PWM_Init( void );	//��ʼ��PWM���
        void   Capture_Init( void );   //��ʼ�����벶��
        extern unsigned int period;
        extern unsigned int pos_time;
        extern char flag;
#endif