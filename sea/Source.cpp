#include <iostream> // ����-�����
#include<algorithm> // ������� ��� �������
#include <Windows.h> // ������� ������ � ����� ������
using namespace std;

enum colors //������������ ������ ��� SetColor
{
	BlueDark, BlueGrey2, GreenGrey, BlueGrey, RedBlack, VioletLite, BrownLite, GreyLite, Grey, Blue, Green, GreenLite, Brown, Violet, Salat, White
};
void Set_Font_Special(const wchar_t* text, int size) //������� ��������� ������ ��� �������
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX fontInfo;
	// ��� ������ �����
	fontInfo.cbSize = sizeof(fontInfo);
	GetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);
	// ��� ������������ ������������� �������
	//wcsncpy(L"Lucida Console", fontInfo.FaceName, LF_FACESIZE);
	wcscpy_s(fontInfo.FaceName, text);
	fontInfo.dwFontSize.Y = size;
	fontInfo.dwFontSize.X = size;
	SetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);
}
void SetColor(int text, int bg) //������� ������������ ����� ������ �������
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}
//char desk[10][10];//������� �����
//char desk_copy[10][10];   //��������������� �����
int cube[8][2] // ��� �������� ����������� ������ ����� ������ �������
{
	{-1,0},
	{-1,-1},
	{0,-1},
	{1,-1},
	{1,0},
	{1,1},
	{0,1},
	{-1,1},
};
struct Ship //���������� �������
{
	char(*ship_map)[10];// ��������� �� ���� ������ ��� �������
	char ship_map_copy[10][10];// ����� ���� ������� ��� ������� ��������
	char empty = static_cast<char>(178); // �������� ������� ��������� ������
	char filled = static_cast<char>(176);// �������� ������� ������� ������
	char damaged = 'X';// �������� ������� ������ ������
	char exists = '+';// �������� ������ ����� ���������� ��������
	int size;// ���������� ����� �������
	enum { one = 1, two, three, four };// ������������ ��� ���������� �����
	int type; // ��� �������
	int damage;// ���������� ������������ �����
	bool status;// ������ ������� (���/�����)
	int orientation;//��������� ������� ������������ ���������
	Ship(int _type, char temp_map[][10])//�����������
	{
		ship_map = temp_map;//������� ����� ������ � ������ �������
		type = _type;
		create(type);
	}
	void maky_copy(char dest[][10], char source[][10]) //������� ����������� ���� 
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				dest[i][j] = source[i][j];
	}
	void clear_desk(char dest[][10]) //������� ����
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				dest[i][j] = empty;
	}
	bool try_create(int _size)//������� ������� �������� �������
	{
		size = _size;
		int x = random_func();
		int y = random_func();
		clear_desk(ship_map_copy);
		maky_copy(ship_map_copy, ship_map);
		orientation = rand() % 4;//����� ������������ �������
		for (int i = 0; i < size; i++)//�������� �� ��������� ����� ����� ��������� �������
		{
			switch (orientation)
			{

			case 0://����� �������
				if (ship_map[x][y + i] == filled || y + size > 10)
					return false;
				for (int k = 0; k < 8; k++)
				{
					if (ship_map[x + cube[k][0]][y + i + cube[k][1]] == filled)
					{
						return false;
					}
				}
				ship_map_copy[x][y + i] = filled;
				break;
			case 1://������ ������
				if (ship_map[x][y - i] == filled || y - size < 0)
					return false;
				for (int k = 0; k < 8; k++)
				{
					if (ship_map[x + cube[k][0]][y - i + cube[k][1]] == filled)
					{
						return false;
					}
				}
				ship_map_copy[x][y - i] = filled;
				break;
			case 2://����� �����
				if (ship_map[x + i][y] == filled || x + size > 10)
					return false;
				for (int k = 0; k < 8; k++)
				{
					if (ship_map[x + i + cube[k][0]][y + cube[k][1]] == filled)
					{
						return false;
					}
				}
				ship_map_copy[x + i][y] = filled;
				break;
			case 3://������ ����
				if (ship_map[x - i][y] == filled || x - size < 0)
					return false;
				for (int k = 0; k < 8; k++)
				{
					if (ship_map[x - i - cube[k][0]][y + cube[k][1]] == filled)
					{
						return false;
					}
				}
				ship_map_copy[x - i][y] = filled;
				break;
			default:
				return false;
				break;
			}
		}
		maky_copy(ship_map, ship_map_copy);//����������� �� ����� � ���������
		return true;
	}
	void create(int _size) //������� ��������� �������
	{
		while (!try_create(_size));
	}
	int random_func()//������� ������� �� 0 �� 9
	{
		int result;
		result = rand() % 10;
		return result;
	}
};

struct Player //��������� ������
{

	static int commonPlayerID;//����������� ���������� ���������� �������
	int ownPlayerID;//ID ������
	int count_of_ships = 0;//���������� ��������
	char desk[10][10];//�������� �����
	char desk_copy[10][10];   //��������������� �����

	void create_ship(int _type)//������ �������� ��������
	{
		for (int i = 5 - _type; i > 0; i--)
		{
			new Ship(_type, desk);
			count_of_ships++;
		}
	}
	static void setPlayerID()//������� ���������� ID ��� �������� ������
	{
		commonPlayerID++;
	}
	Player()//����������� ������
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				desk[i][j] = static_cast<char>(178);
		for (int i = 4; i > 0; i--)
			create_ship(i);
		print();
		setPlayerID();//���������� ID
		ownPlayerID = commonPlayerID;// ���������� ID
	}
	void print()//����� ���� ������
	{
		char space;
		cout << "\n\t";

		cout << " 0 1 2 3 4 5 6 7 8 9\n";
		for (int i = 0; i < 10; i++)
		{
			cout << "\t" << i;
			for (int j = 0; j < 10; j++)//�������� ������ ��� ������������ �������� ����
			{
				if (desk[i][j] == static_cast<char>(176))
				{
					space = static_cast<char>(176);
					SetColor(Green, 0);
				}
				else if (desk[i][j] == 'X')
				{
					space = 'X';
				}
				else
				{
					space = static_cast<char>(178);

					SetColor(White, 0);
				}
				cout << desk[i][j] << space;
			}
			SetColor(White, 0);
			cout << i;
			cout << "\n";
		}
		cout << "\t  0 1 2 3 4 5 6 7 8 9\n";
	}
};
int Player::commonPlayerID = 0;

struct Game//��������� ����
{
	Player* player_1;//����� 1
	Player* player_2;//����� 2
	Game()//����������
	{
		Set_Font_Special(L"Cascadia Mono", 16);//����� ������
		srand((int)time(NULL));//������������� �������
		player_1 = new Player();// ��������� ��������� �� ������
		player_2 = new Player();// �������� ��������� �� ������
	}
	void PrintID()// ����� ID �������
	{

		cout << player_1->ownPlayerID << "\n";
		cout << player_2->ownPlayerID << "\n";
	}

};
int main()
{
	Game myGame;//�������� ����
	myGame.PrintID();//����� ID �������



	return 0;
}