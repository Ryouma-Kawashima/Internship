#pragma once

#include <type_traits>

#define SP_CAST(t) static_cast<##t&>(this)

// ���ÓI�|�����[�t�B�Y������������Ƃ��́A�h�����̃N���X��SP_CAST���g��ꂽ�֐���K����������
