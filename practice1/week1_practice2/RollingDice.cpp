#include "pch.h"
#include "tipsware.h"
#include "time.h"
#define _USE_MATH_DEFINES
#include "math.h"

NOT_USE_MESSAGE

#define MAX_TRY 10000

int main()
{
	//srand(time(NULL));

	SelectPenObject(RGB(0, 0, 0));

	int hit = 0;
	int miss = 0;
	float relProb = 5.0 / 36.0;
	float prob = 0;

	// TODO 

	for (int i = 0; i < MAX_TRY; i++) {
		int num1 = rand() % 6 + 1;
		int num2 = rand() % 6 + 1;

		if (num1 + num2 == 8) {
			TextOut(120 * (i / 20), 20 * (i % 20), "try %d: %d,%d (hit!)", 0 + i + 1, num1, num2);
			hit++;
		}
		else {
			TextOut(120 * (i / 20), 20 * (i % 20), "try %d: %d,%d", 0 + i + 1, num1, num2);
			miss++;
		}
	}
	
	prob = (float)hit / (hit + miss);

	//

	SetTextColor(RGB(128, 0, 0));
	// 실제 주사위 값
	TextOut(0, 420, "실제 값: %.6f", relProb);
	// 계산된 값
	TextOut(0, 440, "계산된 값: %.6f", prob);

	ShowDisplay();

	return 0;
}
