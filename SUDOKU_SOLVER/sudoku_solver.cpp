#include <iostream> //
#include <iomanip>

using namespace std;

// Function for checking the row for the number
bool isSafeInRow(int grid[9][9], int row, int num)
{
    for (int col = 0; col < 9; col++)
    {
        // Check whether the number already exists.
        if (grid[row][col] == num)
        {
            return false;
        }
    }

    return true;
}

//Function for checking the column for the number
bool isSafeInColumn(int grid[9][9], int col, int num)
{
    for (int row = 0; row < 9; row++)
    { // Check whether the number already exists.
        if (grid[row][col] == num)
        {
            return false;
        }
    }

    return true;
}

// Function for checking the 3x3 subgrid
bool isSafeInBox(int grid[9][9], int row, int col, int num)
{ 
    // Calculate the starting row and column of the 3x3 subgrid
    int boxRow = (row / 3) * 3;
    int boxCol = (col / 3) * 3;

    // Check whether the number already exists in the 3x3 subgrid.
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (grid[boxRow + i][boxCol + j] == num)
            {
                return false;
            }
        }
    }

    return true;
}

// Function to check if it's safe to place a number in a cell
bool isSafe(int grid[9][9], int row, int col, int num)
{
    return isSafeInRow(grid, row, num) &&
           isSafeInColumn(grid, col, num) &&
           isSafeInBox(grid, row, col, num);
}

// Function to solve the Sudoku puzzle using backtracking
bool solveSudoku(int grid[9][9])
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (grid[row][col] == 0)
            {
                // We have found an empty cell.
                  for (int num = 1; num <= 9; num++)
                {
                    if (isSafe(grid, row, col, num))
                    {
                        // Place the number.
                        grid[row][col] = num;

                        // Try to solve the remaining puzzle.
                        if (solveSudoku(grid))
                        {
                            return true;
                        }

                        // The choice failed. Backtrack.
                        grid[row][col] = 0;
                    }
                }

                // No number worked for this empty cell.
                return false;
            }
        }
    }

    // No empty cells remain. The puzzle is solved.
    return true;
}

// Function to check if the Sudoku grid is valid
bool isValidSudoku(int grid[9][9])
{
    for (int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 9; col++)
        {
            if (grid[row][col] != 0)
            {
                // Validate the filled cell here.
                int num = grid[row][col];
                grid[row][col] = 0;
                // Check if the number can be placed back in the cell.
                if (!isSafe(grid, row, col, num))
                {
                    return false;
                }
                grid[row][col] = num;
            }
        }
    }

    return true;
}

int main(){

    // Display welcome message
    cout << "====================================================\n";
    cout << "              SUDOKU SOLVER PROGRAM                  \n";
    cout << "====================================================\n\n";

    // Initialize the Sudoku grid
    int grid[9][9]= {0};
    cout << "Enter the Sudoku puzzle (use 0 for empty cells):\n";

int num;

for (int row = 0; row < 9; row++)
{
    for (int col = 0; col < 9; col++)
    {
        int num;

        cout << "Enter value for row " << row + 1
             << ", column " << col + 1 << ": ";

        cin >> num;

        while (cin.fail() || num < 0 || num > 9)
        {
            cin.clear();
            cin.ignore(1000, '\n');

            cout << "Invalid input! Enter a number from 0 to 9: ";
            cin >> num;
        }

        grid[row][col] = num;
    }
}
    
int originalGrid[9][9];

for (int row = 0; row < 9; row++)
{
    for (int col = 0; col < 9; col++)
    {
        originalGrid[row][col] = grid[row][col];
    }
}

// Check if the Sudoku puzzle is valid and solve it
if (!isValidSudoku(grid))
{
    cout << "Invalid Sudoku puzzle! Duplicate numbers found.\n";
}
// Solve the Sudoku puzzle and display the solution
else if (solveSudoku(grid))
{
    cout << "\n========== ORIGINAL SUDOKU ==========\n\n";
    // Display the original Sudoku grid
  // Top border
    cout << "+-------+-------+-------+\n";

    for (int row = 0; row < 9; row++)
    {
        cout << "| ";

        for (int col = 0; col < 9; col++)
        {
            cout << originalGrid[row][col] << " ";

            if (col == 2 || col == 5)
            {
                cout << "| ";
            }
        }

        cout << "|\n";

        if (row == 2 || row == 5)
        {
            cout << "+-------+-------+-------+\n";
        }
    }

    // Bottom border
    cout << "+-------+-------+-------+\n";

    cout << "\n========== SOLVED SUDOKU ==========\n\n";

    // Top border
    cout << "+-------+-------+-------+\n";

    for (int row = 0; row < 9; row++)
    {
        cout << "| ";

        for (int col = 0; col < 9; col++)
        {
            cout << grid[row][col] << " ";

            if (col == 2 || col == 5)
            {
                cout << "| ";
            }
        }

        cout << "|\n";

        if (row == 2 || row == 5)
        {
            cout << "+-------+-------+-------+\n";
        }
    }

    // Bottom border
    cout << "+-------+-------+-------+\n";
}
else
// If the Sudoku puzzle cannot be solved, display a message
{
    cout << "No solution exists.\n";
}

  
    return 0;
}
