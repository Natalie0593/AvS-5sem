#pragma once
#include <iostream>
using namespace std;

void fun_asm(_int8* A, _int8* B, _int8* C, _int16* D)
{
	__int16 res[8] = { 0 };

	for (int i = 0; i < 8; i++)
	{
		res[i] = A[i] - B[i] + C[i] * D[i];
	}


	__int16 F[8] = { 0 };

	__asm {

		//movq mm4, [D]
		//movq mm5,[D + 2 * 4]

		mov ESI, D
		movq mm4, [ESI]
		mov ESI, D
		movq mm5, [ESI + 2 * 4]

		//movq mm2,[C]
		mov ESI, C
		movq mm2, [ESI]
		//распаковать С в mm4,mm5
		movq mm0, mm2
		pcmpgtb mm1, mm0
		punpcklbw mm0, mm1
		movq mm6, mm0//
		movq mm0, mm2
		pcmpgtb mm1, mm0
		punpckhbw mm0, mm1
		movq mm7, mm0//
		/// < C * D >
		pmullw mm4, mm6//первые 4
		pmullw mm5, mm7//последние 4
		/// </summary>

	//	mov eax,[A]
	//	movq mm2,eax

	mov ESI, A
	movq mm2, [ESI]

	//movq mm2, [A]
	//movq mm3, [B]
	mov ESI, B
	movq mm3, [ESI]

	psubb mm2, mm3//A - B рез в mm2

	/// < РАСПАКОВКА mm2 >
	movq mm0, mm2
	pcmpgtb mm1, mm0
	punpcklbw mm0, mm1
	movq mm3, mm0// первые четыре в mm3
	movq mm0, mm2
	pcmpgtb mm1, mm0
	punpckhbw mm0, mm1
	movq mm7, mm0// последние четыре в mm7

	paddw mm3, mm4
	paddw mm7, mm5

	movq[F], mm3
	movq[F + 2 * 4], mm7 
	}

	for (int i = 0; i < 8; i++)
	{
		cout << *(F + i) << " ";
	}

	cout << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << res[i] << " ";
	}
}

void Test_1() {
	__int8 A[8]{ 134, -110, -109, 11, -18, 58, 5, 11 };
	__int8 B[8]{ 150, -4, -2, 0, -7, 3, -10, 10 };
	__int8 C[8]{ 9, 7, 6, 5, 4, 1, 2, 1 };
	__int16 D[8]{ 30000, 4000, -5, -6000, 7, 8, 9, -10 };
	fun_asm(A, B, C, D);
}

int main() {

	Test_1();


	return 0;

}