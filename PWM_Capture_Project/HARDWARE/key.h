#ifndef   __KEY_H_
    #define   __KEY_H_
	//ȡ������
        #define   KEY_L     (!( P1IN & BIT1 ))  //���������Ч��1
        #define   KEY_R     (!( P2IN & BIT1 ))	//�Ҽ�
	//������ֵ����	
	enum	key_type{NOKEY,KEYLEFT,KEYRIGHT};

	extern void	        KeyInit(void);		//���̳�ʼ��
	extern enum key_type 	KeyScan(void);	//����ɨ�躯����������ĳ�����ֵ
#endif