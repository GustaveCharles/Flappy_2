
#include "P_Score.h"
#include "numbers.h"

int score = 0, max_score = 0;

void updateScore(int gp_score)
{
    score = gp_score;
	//Display the score
	displayScore();

	//Update highest score
	if(score > max_score)
	{
		max_score = score;
        displayMaxScore();
	}
}

void displayScore()
{
    int j, i, number;
    // i, digit to display
    for (i = 5; i > 0; i--)
    {
        // calculate the power of i-1 in j
        j = i - 1;
        number = 1;
        while (j--)
            number = number * 10;
        // Obtain the number to render
        number = score / number;
        number = number % 10;
        // Render the number
        BG_MAP_RAM_SUB(24)
        [1 + 2 * 32 + 5-i] = numbersMap[ number * 2] | TILE_PALETTE(8);
        BG_MAP_RAM_SUB(24)
        [1 + (3) * 32 + 5-i] = numbersMap[ number * 2 + 1] | TILE_PALETTE(8);
    }
}

void displayMaxScore()
{
    int i, j, number;
    for (i = 5; i > 0; i--)
    {
        j = i - 1;
        number = 1;
        while (j--)
            number = number * 10;
        number = max_score / number;
        number = number % 10;
        BG_MAP_RAM_SUB(24)
        [24 + 2 * 32 + 5-i] = numbersMap[number * 2] | TILE_PALETTE(8);
        BG_MAP_RAM_SUB(24)
        [24 + 3 * 32 + 5-i] = numbersMap[ number * 2 + 1] | TILE_PALETTE(8);
    }
}