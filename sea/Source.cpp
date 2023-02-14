#include <iostream> // ввод-вывод
#include<algorithm> // функции для консоли
#include <Windows.h> // функции шрифта и цвета текста
using namespace std;

enum colors //Перечисление цветов для SetColor
{
	BlueDark, BlueGrey2, GreenGrey, BlueGrey, RedBlack, VioletLite, BrownLite, GreyLite, Grey, Blue, Green, GreenLite, Brown, Violet, Salat, White
};
void Set_Font_Special(const wchar_t* text, int size) //Функция установки шрифта для консоли
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX fontInfo;
	// эта строка нужна
	fontInfo.cbSize = sizeof(fontInfo);
	GetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);
	// это неправильное использование функции
	//wcsncpy(L"Lucida Console", fontInfo.FaceName, LF_FACESIZE);
	wcscpy_s(fontInfo.FaceName, text);
	fontInfo.dwFontSize.Y = size;
	fontInfo.dwFontSize.X = size;
	SetCurrentConsoleFontEx(hConsole, TRUE, &fontInfo);
}
void SetColor(int text, int bg) //Функция установления цвета текста консоли
{
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}
//char desk[10][10];//Основна доска
//char desk_copy[10][10];   //Вспомогательная доска
int cube[8][2] // Куб проверки свободности клеток возле ячейки корабля
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
struct Ship //Струкутура корабля
{
	char(*ship_map)[10];// указатель на поле игрока для корабля
	char ship_map_copy[10][10];// копия поля корабля при попытке создании
	char empty = static_cast<char>(178); // Значение символа свободной ячейки
	char filled = static_cast<char>(176);// Значение символа занятой ячейки
	char damaged = 'X';// Значение символа убитой ячейки
	char exists = '+';// Значение пустой ячеки пораженной снарядом
	int size;// Количество палуб корабля
	enum { one = 1, two, three, four };// Перечисление для количества палуб
	int type; // Тип корабля
	int damage;// Количество поврежденных палуб
	bool status;// Статус корабля (жив/мертв)
	int orientation;//Положение корабля относительно координат
	Ship(int _type, char temp_map[][10])//Конструктор
	{
		ship_map = temp_map;//Перенос карты игрока в объект корабля
		type = _type;
		create(type);
	}
	void maky_copy(char dest[][10], char source[][10]) //функция копирования поля 
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				dest[i][j] = source[i][j];
	}
	void clear_desk(char dest[][10]) //очистка поля
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				dest[i][j] = empty;
	}
	bool try_create(int _size)//функция попытки создания корабля
	{
		size = _size;
		int x = random_func();
		int y = random_func();
		clear_desk(ship_map_copy);
		maky_copy(ship_map_copy, ship_map);
		orientation = rand() % 4;//выбор напрпавления корабля
		for (int i = 0; i < size; i++)//проверка на свободное место перед созданием корабля
		{
			switch (orientation)
			{

			case 0://Слева направо
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
			case 1://справа налево
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
			case 2://снизу вверх
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
			case 3://сверху вниз
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
		maky_copy(ship_map, ship_map_copy);//копирование из копии в продуктив
		return true;
	}
	void create(int _size) //функция создлания корабля
	{
		while (!try_create(_size));
	}
	int random_func()//Функция рандома от 0 до 9
	{
		int result;
		result = rand() % 10;
		return result;
	}
};

struct Player //Структура игрока
{

	static int commonPlayerID;//статическая переменная количества игроков
	int ownPlayerID;//ID игрока
	int count_of_ships = 0;//Количество кораблей
	char desk[10][10];//Основная доска
	char desk_copy[10][10];   //Вспомогательная доска

	void create_ship(int _type)//Фунция создания кораблей
	{
		for (int i = 5 - _type; i > 0; i--)
		{
			new Ship(_type, desk);
			count_of_ships++;
		}
	}
	static void setPlayerID()//Функция увеличения ID при создании игрока
	{
		commonPlayerID++;
	}
	Player()//Конструктор Игрока
	{
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				desk[i][j] = static_cast<char>(178);
		for (int i = 4; i > 0; i--)
			create_ship(i);
		print();
		setPlayerID();//Увеличение ID
		ownPlayerID = commonPlayerID;// Присвоение ID
	}
	void print()//Вывод поля игрока
	{
		char space;
		cout << "\n\t";

		cout << " 0 1 2 3 4 5 6 7 8 9\n";
		for (int i = 0; i < 10; i++)
		{
			cout << "\t" << i;
			for (int j = 0; j < 10; j++)//Удвоение ячейки для гармонизации игрового поля
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

struct Game//Структура игры
{
	Player* player_1;//Игрок 1
	Player* player_2;//Игрок 2
	Game()//Конструтор
	{
		Set_Font_Special(L"Cascadia Mono", 16);//Выбор шрифта
		srand((int)time(NULL));//Инициализация рандома
		player_1 = new Player();// Созддание указателя на игрока
		player_2 = new Player();// Создание укащателя на игрока
	}
	void PrintID()// Вывод ID игроков
	{

		cout << player_1->ownPlayerID << "\n";
		cout << player_2->ownPlayerID << "\n";
	}

};
int main()
{
	Game myGame;//Создание игры
	myGame.PrintID();//Вывод ID игроков



	return 0;
}