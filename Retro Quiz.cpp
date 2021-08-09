#include "pch.h"

int width = 0;
int height = 0;
HANDLE hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
int bufferinfo() //pobieranie informacji o buforze okna
{
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

	GetConsoleScreenBufferInfo(hConsoleOut, &csbiInfo);
	width = csbiInfo.dwSize.X;
	height = csbiInfo.dwSize.Y;

	return 0;
}

std::string fileContent[15];
void readFile() { //Odczytywanie pliku z menu i wypisywanie go na ekran
	system("cls");
	std::fstream file;
	file.open("Menu.txt", std::ios::in);
	std::string line;
	if (file.good() == true)
	{
		for (int i = 0; i < 15; i++)
		{
			std::getline(file, line);
			fileContent[i] = line;
		}
	}
	else
	{
		system("Color 0C");
		std::cout << "Nie można odczytać pliku...";
	}
	file.close();
}

std::string center(const std::string s, const int w) { //wyśrodkowywanie tekstu względem bufora
	std::stringstream ss, spaces;			//strumień umożliwiający zapis i odczyt
	int pad = w - s.size();                  // obliczanie ilości miejsca pozostałej po odjęciu naszego stringa od szerokości bufora
	for (int i = 0; i < pad / 2; ++i)
		spaces << " ";							//zapisywanie do strumienia odpowiednią ilość spacji potrzebnej do wysrodkowania napisu z jednej strony
	ss << spaces.str() << s << spaces.str(); // tworzenie finalnego, drugiego strumienia z użyciem poprzednio wygenerowanej, odpowiedniej ilości spacji, po jednej i po drugiej ze stron
	if (pad > 0 && pad % 2 != 0)                    // jeżeli ilość pustego miejsca jest nieparzysta, dodajemy jedną spację, żeby wyrównać z liniamii parzystymi
		ss << " ";
	return ss.str();	//zwrócenie gotowego napisu, już wyśrodkowanego
}

CONSOLE_SCREEN_BUFFER_INFO screenInfo; //pobieranie informacji o buforze ekranu
void consoleSettings()
{
	SendMessage(GetConsoleWindow(), WM_SYSKEYDOWN, VK_RETURN, 0x20000000); //maksymalizacja okna konsoli, względem bufora, wysyła komunikat do procedury danego okna
	setlocale(LC_ALL, "Polish"); //ustawienie pozwalające nam na użycie polskich znaków, jednakże tylko i wyłącznie z  poziomu programu, ponieważ to ustawienie nie działa na plikach :/
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenInfo); //pobieranie informacji o buforze okna konsoli
	HWND x = GetConsoleWindow(); //tworzy uchwyt do okna konsoli
	ShowScrollBar(x, SB_BOTH, FALSE); //usuwa nam prawy i dolny scrollbar
}

void setFontSize(int FontSize) //ustawienia czcionki
{
	CONSOLE_FONT_INFOEX info = { 0 }; //umożliwia wprowadzanie zmian w czcionce konsoli i ustawienie jej parametrów
	info.cbSize = sizeof(info); //zapisywanie rozmiaru struktury
	info.dwFontSize.Y = FontSize; //ustalenie wielkości czcionki
	info.FontWeight = FW_NORMAL; //ustawienie pogrubienia czcionki
	wcscpy_s(info.FaceName, L"Lucida Console"); //ustawienia czcionki 
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info); //zapisanie powyższych usatwień na stałe
}
void changeColor(short int code) //zmiana koloru wyświetlanego tekstu
{
	SetConsoleTextAttribute(hConsoleOut, code); //funkcja umożliwiająca zmianę
}

bool wasDrawn(int num, int tab[], int howMuch) //sprawdzenie czy podana liczba znajduje już się w tablicy, czyli sprawdzenie powtórzeń danej liczby
{
	if (howMuch <= 0)
		return false;
	else
	{
		int i = 0;
		do
		{
			if (tab[i] == num)
				return true;

			i++;
		} while (i < howMuch);

		return false;
	}
}

int random(short int range) //losowanie liczby, z zakresu 0-n
{
	return (std::rand() % range);
}
bool is_empty(std::fstream& pFile) //sprawdzenie, czy plik przypadkiem nie jest pusty
{
	return pFile.peek() == std::fstream::traits_type::eof();
}

void statistics(float percentage, int points, int size) //funkcja odpowiadająca za statystyki
{
	bool emptyFile;
	std::fstream statistic("Statistics.txt", std::ios::in);
	emptyFile = is_empty(statistic);
	statistic.close();
	statistic.open("Statistics.txt", std::ios::out|std::ios::app);
	if (!statistic.good())
	{
		changeColor(4);
		std::cout << center("Zła ścieżka do pliku, nie może być odczytany!", screenInfo.dwSize.X);
	}
	else
	{

		if(emptyFile)
		{
			statistic << percentage << "% " << points << "/" << size;
		}
		else
		{
			statistic.seekp(0, std::ios::end);
			statistic << "\n" << percentage << "% " << points << "/" << size;
		}
	}
	statistic.close();
}

void readingQuestions(int questionAmount, int randomNumbers[], std::string questions[], std::string anwsers[], std::string correctAnwsers[])
//funckja służąca do oczytu pytań, odpowiedzi i poprawnych odpowiedzi z pliku tekstowego
{
	std::fstream question;
	question.open("Questions.txt", std::ios::in);
	for (int i = 0; i < questionAmount; i++)
	{
		int temp = randomNumbers[i]*3;
		question.seekg(std::ios::beg);
		for (int j = 0; j < temp; j++)
		{
			question.ignore(10000, '\n');
		}	
		std::getline(question, questions[i]);
		std::getline(question, anwsers[i]);
		std::getline(question, correctAnwsers[i]);
	}
	question.close();

}
void readingStatistics()
//funkcja odczytująca plik ze statystykami
{
	int c = 0;
	int j;
	std::string line;
	std::fstream f;
	std::string stats[3];
	f.open("Statistics.txt", std::ios::in);
	if (!f.good()) {
		changeColor(4);
		std::cout << center("Zła ścieżka do pliku, nie może być odczytany!", screenInfo.dwSize.X);
	}
	else {
		while (!f.eof()) {
			getline(f, line);
			c++;
		}
		c -= 3;
		f.seekg(std::ios::beg);
		for (j = 0; j < c; j++)
		{
			f.ignore(10000, '\n');
		}
		for (j = 0; j < 3; j++)
		{
			if (!f.eof())
				std::getline(f, stats[j]);
			else
				break;
		}
		for (int k = 0; k < j; k++)
		{
			std::cout << center(stats[k], screenInfo.dwSize.X);
		}
	}
	f.close();
}
int countFileSize(std::string fileName)
{
	int number_of_lines = 0;
	std::string line;
	std::ifstream myFile(fileName);

	while (std::getline(myFile, line))
		++number_of_lines;
	return number_of_lines;
}
int main()
{
	setFontSize(35);
	consoleSettings();
	srand(time(NULL));
	for (;;)
	{
		system("cls");
		readFile();

		for (int i = 0; i < 14; i++)
		{
			changeColor(i);
			std::cout << center(fileContent[i], screenInfo.dwSize.X) << std::endl;
		}
		changeColor(4);
		std::cout << std::endl;
		std::cout << center("\x18\x18\x18 Wybierz opcje klikając odpowiednie liczby na klawiaturze \x18\x18\x18", screenInfo.dwSize.X);
		char chosen = _getch();
		switch (chosen)
		{
		case '1':
		{
			Beep(500, 200);
			system("cls");
			changeColor(6);
			std::cout << center("Ile pytan ma byc zawarte w quizie?", screenInfo.dwSize.X);
			int questionAmount;
			bool success;
			int fileSize = countFileSize("Questions.txt")/3;
			do {
				changeColor(6);
				std::cout << "Wpisz ilość pytań (max"<<fileSize<<"):"<<std::endl;
				std::cout << std::endl;
				changeColor(4);				changeColor(5);
				success = true;
				std::cin >> questionAmount;
				if (std::cin.fail())
				{
					success = false;
					std::cin.clear();
					std::cin.ignore(INT_MAX, '\n');
					changeColor(4);
					std::cout << center("Zły format odpowiedzi, proszę wpisać liczbę...", screenInfo.dwSize.X);
				}
				if ((fileSize*3)%3!=0)
				{
					success = false;

					changeColor(4);
					std::cout << center("Błąd w pliku 'Questions.txt'", screenInfo.dwSize.X);
				}
				if (questionAmount>fileSize)
				{
					success = false;

					changeColor(4);
					std::cout << "Proszę wprowadzić liczbę z zakresu od 1 do " << fileSize<< std::endl;
				}
				else
				{
					success = true;
				}
			}
			while (success == false);
			changeColor(4);

				Beep(500, 200);
				std::string* questions = new std::string[questionAmount];
				std::string* anwsers = new std::string[questionAmount];
				std::string* correctAnwser = new std::string[questionAmount];
				int* randomNumbers = new int[questionAmount];
				int size = questionAmount;
				int currentQuestion = 0;
				int j = 0;
				char anwser;
				int points = 0;
				float part=static_cast<float>(1)/ questionAmount;
				float percentage = 0;
				do
				{
					int number = random(31);
					if (wasDrawn(number, randomNumbers, j) == false)
					{
						randomNumbers[j] = number;
						j++;
					}
				} while (j < size);
				readingQuestions(size, randomNumbers, questions, anwsers, correctAnwser);				
				changeColor(9);
				system("cls");
				std::cout << center("Trwa ładowanie pytań: 0%", screenInfo.dwSize.X); Sleep(600); system("cls"); Sleep(600); std::cout << center("Trwa ładowanie pytań: 20%", screenInfo.dwSize.X); system("cls"); Sleep(600); std::cout << center("Trwa ładowanie pytań: 56%", screenInfo.dwSize.X); system("cls"); Sleep(600); std::cout << center("Trwa ładowanie pytań: 89%", screenInfo.dwSize.X); system("cls"); Sleep(1000); std::cout << center("Trwa ładowanie pytań: 100%", screenInfo.dwSize.X);				
				for(int k = 0; k < size; k++)
				{
					currentQuestion++;
					system("cls");
					changeColor(8);
					std::cout<<center(questions[k], screenInfo.dwSize.X);
					std::cout << center(anwsers[k], screenInfo.dwSize.X);
					changeColor(9);
					std::cout <<"Twoje punkty:" << points << "/" << size<<std::endl;
					changeColor(12);
					std::cout << "Aktualne pytanie:" << currentQuestion << "/" << size << std::endl;
					char* c = const_cast<char*>(correctAnwser[k].c_str());
					anwser = _getch();
					if (anwser != 'a' && anwser != 'b' && anwser != 'c')
					{
						Beep(200, 600);
						changeColor(4);
						std::cout << center("Zły format odpowiedzi, proszę wybrać spośród: a/b/c...", screenInfo.dwSize.X);
						anwser = _getch();
					}
					if (anwser == c[0])
					{
						Beep(500, 200);
						changeColor(2);
						std::cout << center("Poprawna odpowiedź!", screenInfo.dwSize.X);
						percentage = percentage + part;
						points++;
						Sleep(1500);
					}
					else
					{
						Beep(200, 600);
						changeColor(4);
						std::cout << center("Niepoprawna odpowiedź!", screenInfo.dwSize.X);
						Sleep(1500);
					}
				}
				percentage = percentage * 100;
				statistics(percentage, points, size);
				changeColor(2);
				system("cls");
				std::cout << "Brawo!!!"<<std::endl;
				changeColor(9);
				std::cout << "Ukończyłeś quiz z poprawnymi odpowiedziami na poziomie : " << percentage << " % " << std::endl;
				std::cout << "Twoje punkty:" << points << "/" << size << std::endl;
				std::cout << center("\x18\x18\x18 Naciśnij dowolny klawisz, aby powrócić do menu \x18\x18\x18", screenInfo.dwSize.X);
				percentage = 0;
				delete[] questions;
				delete[] anwsers;
				delete[] correctAnwser;
				delete[] randomNumbers;
				char chosenSize = _getch();

		
		}
		break;
			case '2':
			{
				Beep(500, 200);
				system("cls");
				changeColor(11);
				std::cout << center("Statystyki", screenInfo.dwSize.X);
				std::cout << std::endl;
				changeColor(14);
				std::cout << center("Twoje wyniki z ostatnich trzech gier:", screenInfo.dwSize.X);
				changeColor(6);
				readingStatistics();
				changeColor(4);
				std::cout << center("\x18\x18\x18 Naciśnij dowolny klawisz, aby powrócić do menu \x18\x18\x18", screenInfo.dwSize.X);
				char leave = _getch();
			}
		break;

			case '3':
			{
				Beep(500, 200);
				system("cls");
				changeColor(12);
				std::cout << center("Zasady gry", screenInfo.dwSize.X);
				std::cout << std::endl;
				std::cout << std::endl;
				changeColor(7);
				std::cout << center("Nie trzeba się chyba rozpisywać jak działa quiz, odpowiada się na pytania zamknięte, wybierając na klawiaturze odpowiednią literę. Program zlicza punkty, pokazując nam dodatkowo, czy odpowiedź była poprawna, czy też nie.", screenInfo.dwSize.X);
				std::cout << std::endl;
				changeColor(4);
				std::cout << center("\x18\x18\x18 Naciśnij dowolny klawisz, aby powrócić do menu \x18\x18\x18", screenInfo.dwSize.X);
				char leave = _getch();
			}
		break;

			case'4':
				exit(0);;
				break;
		}
	}
}