// -----------------------------------
// Author: Ryouma Kawashima
// Create: 2020/05/18
// Update: 2021/10/04
// -----------------------------------
#include "./debug_printf.h"

#include <stdio.h>
#include <malloc.h>
#include <windows.h>

using namespace DragonLib;

int DragonLib::DebugPrintf(const char* format, ...)
{
#if defined(_DEBUG) || defined(DEBUG)

	size_t	len	= 0;
	char*	buf	= NULL;
	va_list	ap	= NULL;

	va_start(ap, format);
	len = _vscprintf(format, ap) + 1;		// �ϊ���̒������擾(\0���܂܂�Ă��Ȃ��̂�+1)
	buf = (char*)_malloca(len);				// �X�^�b�N�̈�܂��̓q�[�v�̈�Ɋm��
	len = vsnprintf(buf, len, format, ap);	// �ϊ�����(������len�Ɋi�[)
	va_end(ap);

	OutputDebugStringA(buf);
	_freea(buf);

	return (int)len;

#else

	UNREFERENCED_PARAMETER(format);
	return 0;

#endif // _DEBUG || DEBUG
}
