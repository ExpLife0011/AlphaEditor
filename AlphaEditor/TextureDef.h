#ifndef TEXTUREDEF_H__
#define TEXTUREDEF_H__


enum TEXT_STYLE
{
	TEXT_NONE =0,
	TEXT_LINE_FRAME,	// �߿�
	TEXT_ARROW,			// ��ͷ
	TEXT_ELIPSE,		// ��Բ
	TEXT_RECT,			// ����
	TEXT_ROUND,			// Բ��
	TEXT_CLOUDS,		// �ƶ�
	TEXT_BLOW,			// ��ը
};


enum TEXT_STATE
{
	TEXT_STATE_NORMAL=0,		// ���֣�ͼƬ
	TEXT_STATE_FOCUS,			// �༭״̬
	TEXT_STATE_KILLFOCUS,
	TEXT_STATE_MOUSEENTER,		// �������
	TEXT_STATE_MOUSELEAVE,    // ����Ƴ�
	TEXT_STATE_LBUTTONDOWN,   // ����������
	TEXT_STATE_KEYDOWN,			// ��갸������
};




#endif //TEXTUREDEF_H__
