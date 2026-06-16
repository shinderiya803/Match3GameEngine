#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <fstream>
#include <windows.h>
#include <iomanip>

using namespace std;

class CandyCrush
{
private:
    char board[8][8];
    char candies[5] = { 'A','B','C','D','E' };
    int score;
    int movesLeft;
    int targetScore;
    int highScore;

public:

    CandyCrush()
    {
        score = 0;
        movesLeft = 20;
        targetScore = 500;
    }

    void generateBoard();
    void displayBoard();
    void printCandy(char candy);
    void swapCandy(int r1, int c1, int r2, int c2);
    bool isAdjacent(int r1, int c1, int r2, int c2);
    bool checkMatch();
    int removeMatches();
    void applyGravity();
    void fillBoard();
    void play();
    void loadHighScore();
    void saveHighScore();
};
void CandyCrush::generateBoard()
{ srand(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            char newCandy;
            do
            {
                newCandy = candies[rand() % 5];
            } while ( (j >= 2 && board[i][j - 1] == newCandy && board[i][j - 2] == newCandy) || (i >= 2 && board[i - 1][j] == newCandy && board[i - 2][j] == newCandy));
            board[i][j] = newCandy;
        }
    }
}
void CandyCrush::displayBoard()
{
    cout << "\n\n";
    // Column numbers
    cout << "  ";
    for (int j = 0; j < 8; j++)
    {
        cout << setw(2) << j;
    }
    cout << endl;
    cout << "  +----------------+" << endl;
    for (int i = 0; i < 8; i++)
    {
        cout << setw(2)<<i<<"|";

        for (int j = 0; j < 8; j++)
        {
            printCandy(board[i][j]);
        }
        cout << "| "<< endl;
    }
    cout << "  +----------------+" << endl;
    cout << "\nScore = " << score << endl;
    cout << "High Score = " << highScore << endl;
    cout << "Moves Left = " << movesLeft << endl;
    cout << "Target Score = " << targetScore << endl;
}
void CandyCrush::printCandy(char candy)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    switch (candy)
    {
    case 'A':
        SetConsoleTextAttribute(h, 12); // Red
        break;
    case 'B':
        SetConsoleTextAttribute(h, 10); // Green
        break;
    case 'C':
        SetConsoleTextAttribute(h, 9); // Blue
        break;
    case 'D':
        SetConsoleTextAttribute(h, 14); // Yellow
        break;
    case 'E':
        SetConsoleTextAttribute(h, 13); // Purple
        break;
    case '*':
        SetConsoleTextAttribute(h, 7); // White
        break;
    default:
        SetConsoleTextAttribute(h, 7);
    }
    cout << candy << " ";

    // Reset to normal white
    SetConsoleTextAttribute(h, 7);
}
void CandyCrush::swapCandy(int r1, int c1, int r2, int c2)
{
    char temp = board[r1][c1];
    board[r1][c1] = board[r2][c2];
    board[r2][c2] = temp;
}
bool CandyCrush::isAdjacent(int r1, int c1, int r2, int c2)
{
    if ((abs(r1 - r2) == 1 && c1 == c2) || (abs(c1 - c2) == 1 && r1 == r2))
    {
        return true;
    }
    return false;
}
bool CandyCrush::checkMatch()
{ // Horizontal check
    for (int i = 0; i < 8; i++)
    {
        int count = 1;

        for (int j = 1; j < 8; j++)
        {
            if (board[i][j] == board[i][j - 1])
            {
                count++;
            }
            else
            {
                if (count >= 3)
                {
                    return true;
                }

                count = 1;
            }
        }

        // Check sequence ending at last column
        if (count >= 3)
        {
            return true;
        }
    }

    // Vertical check
    for (int j = 0; j < 8; j++)
    {
        int count = 1;

        for (int i = 1; i < 8; i++)
        {
            if (board[i][j] == board[i - 1][j])
            {
                count++;
            }
            else
            {
                if (count >= 3)
                {
                    return true;
                }

                count = 1;
            }
        }
        // Check sequence ending at last row
        if (count >= 3)
        {
            return true;
        }
    }
    return false;
}
int CandyCrush::removeMatches()
{
    bool mark[8][8] = { false };

    // Horizontal scan
    for (int i = 0; i < 8; i++)
    {
        int count = 1;

        for (int j = 1; j < 8; j++)
        {
            if (board[i][j] == board[i][j - 1] && board[i][j] != '*')
            {
                count++;
            }
            else
            {
                if (count >= 3)
                {
                    for (int k = j - count; k < j; k++)
                    {
                        mark[i][k] = true;
                    }
                }

                count = 1;
            }
        }

        // Check sequence reaching the end of row
        if (count >= 3)
        {
            for (int k = 8 - count; k < 8; k++)
            {
                mark[i][k] = true;
            }
        }
    }

    // Vertical scan
    for (int j = 0; j < 8; j++)
    {
        int count = 1;

        for (int i = 1; i < 8; i++)
        {
            if (board[i][j] == board[i - 1][j] &&  board[i][j] != '*')
            {
                count++;
            }
            else
            {
                if (count >= 3)
                {
                    for (int k = i - count; k < i; k++)
                    {
                        mark[k][j] = true;
                    }
                }

                count = 1;
            }
        }

        // Check sequence reaching the bottom
        if (count >= 3)
        {
            for (int k = 8 - count; k < 8; k++)
            {
                mark[k][j] = true;
            }
        }
    }

    int totalRemoved = 0;

    // Remove marked candies
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (mark[i][j])
            {
                board[i][j] = '*';
                totalRemoved++;
            }
        }
    }

    // Update score
    score += totalRemoved * 10;

    return totalRemoved;
}
void CandyCrush::applyGravity()
{
    // Process each column separately
    for (int j = 0; j < 8; j++)
    {
        // Start writing from the bottom row
        int writeRow = 7;

        // Scan from bottom to top
        for (int i = 7; i >= 0; i--)
        {
            // If current cell contains a candy
            if (board[i][j] != '*')
            {
                // Move the candy to writeRow
                board[writeRow][j] = board[i][j];

                // If the candy moved, empty its old position
                if (writeRow != i)
                {
                    board[i][j] = '*';
                }

                // Move writeRow upward
                writeRow--;
            }
        }

        // Fill the remaining cells at the top with '*'
        while (writeRow >= 0)
        {
            board[writeRow][j] = '*';
            writeRow--;
        }
    }
}
void CandyCrush::fillBoard()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (board[i][j] == '*')
            {
                board[i][j] = candies[rand() % 5];
            }
        }
    }
}
void CandyCrush::loadHighScore()
{
    ifstream fin("highscore.txt");
    if (fin)
    {
        fin >> highScore;
    }
    else
    {
        highScore = 0;
    }
    fin.close();
}
void CandyCrush::saveHighScore()
{
    ofstream fout("highscore.txt");
    fout << highScore;
    fout.close();
}
void CandyCrush::play()
{
    generateBoard();
    loadHighScore();
    int r1, c1, r2, c2;

    char choice = 'Y';

    while ((choice == 'Y' || choice == 'y') && (movesLeft > 0) && ( score < targetScore))
    { displayBoard();
        cout << "\nEnter row and column of first candy (0-7): ";
        cin >> r1 >> c1;
        cout << "Enter row and column of second candy (0-7): ";
        cin >> r2 >> c2;

        // Check bounds
        if (r1 < 0 || r1 > 7 || c1 < 0 || c1 > 7 ||    r2 < 0 || r2 > 7 || c2 < 0 || c2 > 7)
        {
            cout << "\nInvalid positions!\n";
            continue;
        }

        // Check adjacency
        if (!isAdjacent(r1, c1, r2, c2))
        {
            cout << "\nCandies must be adjacent!\n";
            continue;
        }
        // Perform swap
        swapCandy(r1, c1, r2, c2);
        // Check whether swap created a match
        if (!checkMatch())
        {
            // Undo swap
            swapCandy(r1, c1, r2, c2);

            cout << "\nInvalid move! No match formed.\n";
        }
        else
        {
            movesLeft--;
            // Continue until no more matches remain
            while (checkMatch())
            {
                removeMatches();
                applyGravity();
                fillBoard();
            }

            cout << "\nMove successful!\n";
        }

        displayBoard();

        cout << "\nContinue playing? (Y/N): ";
        cin >> choice;
    }
    if (movesLeft == 0)
    {
        cout << "\nNo moves left!\n";
    }

    if (score >= targetScore)
    {
        cout << "\nCongratulations! YOU WIN!\n";
    }
    else if (movesLeft == 0)
    {
        cout << "\nNo moves left!\n";
        cout << "YOU LOSE!\n";
    }
    if (score > highScore)
    {
        highScore = score;
        saveHighScore();
        cout << "\nNEW HIGH SCORE!\n";
    }
    else
    {
        cout << "\nGame Over!\n";
    }

    cout << "Final Score = " << score << endl;
}
int main()
{
    CandyCrush game;

    game.play();

    return 0;
}