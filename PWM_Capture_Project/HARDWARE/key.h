#ifndef   __KEY_H_
    #define   __KEY_H_
	//取按键宏
        #define   KEY_L     (!( P1IN & BIT1 ))  //左键按下有效是1
        #define   KEY_R     (!( P2IN & BIT1 ))	//右键
	//按键键值声明	
	enum	key_type{NOKEY,KEYLEFT,KEYRIGHT};

	extern void	        KeyInit(void);		//键盘初始化
	extern enum key_type 	KeyScan(void);	//键盘扫描函数返回上面的常量键值
#endif