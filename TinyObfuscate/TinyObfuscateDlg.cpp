// TinyObfuscate - By Michael Haephrati (C)2017 Michael Haephrati, Secured Globe, Inc.
// 1501 Broadway STE 1200, New York 10036, NY

#include "stdafx.h"
#include "TinyObfuscate.h"
#include "TinyObfuscateDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <vector>
#include <ctime>
#include "random.h"
#include <fstream>
#include <string>
#include <cstdio>
#include <map>
#include <functional>
#include <regex>
#include <sstream>
#include <random>

#pragma warning(disable:4996)

#define RANDOM_CODE (RandomIntFromRange(40,126))
#define RANDOM_DIGIT (RandomIntFromRange(1,9))
#define RANDOM_WCHAR (WCHAR)(RandomIntFromRange(L'a',L'z'))
#define RANDOM_CHAR (char)(RandomIntFromRange('a','z'))

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int arr[1028];


using namespace std;

vector<string> fileH, fileCPP;
string ftemp, ffunctions, fhfunctions, fproject;

map<string, string> functions;
map<string, string> idens;
map<string, string> numbers;
map<string, string> strings;
map<string, string> bodies;

struct H
{
	string s1, s2;
};


namespace std
{
	template <> struct hash<H>
	{
		size_t operator()(const H& h) const
		{
			return hash<string>{}(h.s1) ^ (hash<string>{}(h.s2) << 1);
		}
	};
}


using namespace std;
CString stringToObfuscate(CString OriginalText, BOOL IsUNICODEChecked);

//check file existing
bool existsFile(const std::string& name) {
	ifstream f(name.c_str());
	return f.good();
}
//copy file (from -> to)
void copyFile(const std::string &from, const std::string &to)
{
	std::ifstream is(from, ios::in | ios::binary);
	std::ofstream os(to, ios::out | ios::binary);

	std::copy(std::istreambuf_iterator<char>(is), std::istreambuf_iterator<char>(),
		std::ostreambuf_iterator<char>(os));
}


//split the string into tokens
vector<string> split(string &s, char sep) {
	stringstream ss(s);
	string item;
	vector<string> tokens;
	while (getline(ss, item, sep)) {
		tokens.push_back(item);
	}
	return tokens;
}

// delete spacebars from start and from end of string
string trim(string s) {

	string left = "";

	// delete spacebars from start
	bool start = false;
	for (auto c : s) {
		if (c == ' ' && !start)
			continue;
		if (c != ' ')
			start = true;

		left += c;
	}


	string result = "";

	// delete spacebars from end
	int i;
	for (i = int(left.size()) - 1; i > 0; i--)
		if (left[i] != ' ')
			break;

	for (int j = 0; j <= i; j++)
		result += left[j];
	return result;
}

void replaceStrings(string fname) {
	std::ifstream in;

	in.open(fname.c_str());
	std::ofstream out;
	out.open(ftemp);
	if (!in)
	{
		TCHAR e[1000] = _T("Error");
		MessageBox(NULL, e, _T("Could not open file in"), NULL);

		//cerr << "Could not open file \n";
		return;
	}

	if (!out)
	{
		TCHAR e[1000] = _T("Error");
		MessageBox(NULL, e, _T("Could not open file out"), NULL);

		//cerr << "Could not open file \n";
		return;
	}

	string line;

	while (getline(in, line))
	{
		size_t p = line.find("//", 0);

		//remove comment
		if (p != string::npos)
			line.resize(p, '$');

		p = line.find("#pragma", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}


		p = line.find("#define", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}

		p = line.find("#include", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}

		for (auto m : strings) {
			string wordToReplace = m.first;
			string wordToReplaceWith = m.second;
			size_t len = wordToReplace.length();

			while (true)
			{
				size_t pos = line.find(wordToReplace);
				if (pos != string::npos) {
					line.replace(pos, len, wordToReplaceWith);
					if (pos > 0 && line[pos - 1] == 'L')
						line.replace(pos - 1, 1, "");
				}
				else
					break;
			}
		}
		out << line << '\n';
	}
	in.close();
	out.close();
	CString fin(fname.c_str());
	DeleteFile(fin);

	rename(ftemp.c_str(), fname.c_str());
}

void replaceFunctions(string fname) {
	std::ifstream in;

	in.open(fname.c_str());
	std::ofstream out;
	out.open(ftemp);
	if (!in)
	{
		TCHAR e[1000] = _T("Error");
		MessageBox(NULL, e, _T("Could not open file in"), NULL);

		//cerr << "Could not open file \n";
		return;
	}

	if (!out)
	{
		TCHAR e[1000] = _T("Error");
		MessageBox(NULL, e, _T("Could not open file out"), NULL);

		//cerr << "Could not open file \n";
		return;
	}

	string line;

	while (getline(in, line))
	{
		size_t p = line.find("//", 0);

		//remove comment
		if (p != string::npos)
			line.resize(p, '$');

		p = line.find("#pragma", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}

		p = line.find("#define", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}

		p = line.find("#include", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}


		for (auto m : functions) {
			string wordToReplace = m.first;
			string wordToReplaceWith = m.second;
			size_t len = wordToReplace.length();

			long beg = 0;
			while (true)
			{


				size_t pos = line.find(wordToReplace, beg);

				if (pos != string::npos) {
					if (!isalpha(line[pos + len]) &&
						!isdigit(line[pos + len]) &&
						//line[pos + len] != ':' &&
						line[pos + len] != '_') {
						if (pos == 0 ||
							(!isalpha(line[pos - 1]) &&
								!isdigit(line[pos - 1]) &&
								//line[pos - 1] != ':' &&
								line[pos - 1] != '_')) {
							line.replace(pos, len, wordToReplaceWith);
						}
					}
				}
				else
					break;
				beg = beg + 1;
			}
		}
		out << line << '\n';
	}
	in.close();
	out.close();
	CString fin(fname.c_str());
	DeleteFile(fin);

	rename(ftemp.c_str(), fname.c_str());
}

void replaceVariables(string fname) {
	std::ifstream in;

	in.open(fname.c_str());
	std::ofstream out;
	out.open(ftemp);
	if (!in || !out)
	{
		TCHAR e[1000] = _T("Error");
		MessageBox(NULL, e, _T("Could not open file"), NULL);
		//cerr << "Could not open file \n";
		return;
	}

	string line;

	while (getline(in, line))
	{

		size_t p = line.find("//", 0);

		//remove comment
		if (p != string::npos)
			line.resize(p, '$');
		p = line.find("#pragma", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}

		p = line.find("#include", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}

		p = line.find("#define", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}

		for (auto m : idens) {
			string wordToReplace = m.first;
			string wordToReplaceWith = m.second;
			size_t len = wordToReplace.length();

			long beg = 0;
			while (true)
			{

				size_t pos = line.find(wordToReplace, beg);
				if (pos != string::npos) {
					if (!isalpha(line[pos + len]) &&
						!isdigit(line[pos + len]) &&
						//line[pos + len] != ':' &&
						line[pos + len] != '_') {
						if (pos == 0 ||
							(!isalpha(line[pos - 1]) &&
								!isdigit(line[pos - 1]) &&
								//line[pos -1] != ':' &&
								line[pos - 1] != '_')) {
							line.replace(pos, len, wordToReplaceWith);
						}
					}
				}
				else
					break;

				beg = beg + 1;
			}
		}
		out << line << '\n';

	}
	in.close();
	out.close();
	CString fin(fname.c_str());
	DeleteFile(fin);

	rename(ftemp.c_str(), fname.c_str());
}

void replaceNumbers(string fname) {
	std::ifstream in;

	in.open(fname.c_str());
	std::ofstream out;
	out.open(ftemp);
	if (!in || !out)
	{
		TCHAR e[1000] = _T("Error");
		MessageBox(NULL, e, _T("Could not open file"), NULL);
		//cerr << "Could not open file \n";
		return;
	}

	string line;

	while (getline(in, line))
	{
		size_t p = line.find("//", 0);

		//remove comment
		if (p != string::npos)
			line.resize(p, '$');

		p = line.find("#include", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}

		p = line.find("#pragma", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}

		p = line.find("#define", 0);
		if (p != string::npos) {
			out << line << '\n';
			continue;
		}

		for (auto m : numbers) {
			string wordToReplace = m.first;
			string wordToReplaceWith = m.second;
			size_t len = wordToReplace.length();

			long beg = 0;
			while (true)
			{
				size_t pos = line.find(wordToReplace, beg);
				if (pos != string::npos) {
					if (!isdigit(line[pos + len]) &&
						!isalpha(line[pos + len]) &&
						line[pos + len] != '_' &&

						line[pos + len] != '\'' &&
						line[pos + len] != '\"' &&

						line[pos + len] != '.') {

						if (pos == 0 ||
							(!isdigit(line[pos - 1]) &&
								!isalpha(line[pos - 1]) &&
								line[pos - 1] != '.' &&
								line[pos - 1] != '\''&&
								line[pos - 1] != '_' &&
								line[pos - 1] != '\"')) {

							line.replace(pos, len, wordToReplaceWith);
							//beg=beg+wordToReplaceWith.size()-1;
						}
					}
				}
				else
					break;
				beg = beg + 1;
			}
		}
		out << line << '\n';
	}
	in.close();
	out.close();
	CString fin(fname.c_str());
	DeleteFile(fin);

	rename(ftemp.c_str(), fname.c_str());

}

void AddExistingFilesToProject() {
	string fname = fproject;
	std::ifstream in;
	in.open(fname.c_str());

	std::ofstream out;
	out.open(ftemp);
	if (!in || !out)
	{
		cerr << "Could not open file \n";
		return;
	}

	string line;

	bool flagCPP = false;
	bool flagH = false;
	while (getline(in, line))
	{
		out << line << '\n';

		if (!flagCPP) {
			size_t pos = line.find("<ClCompile Include=");
			if (pos != string::npos) {

				out << "<ClCompile Include=\"" << "functions_AddToProject.cpp\" />\n";
				flagCPP = true;
			}
		}
		if (!flagH) {
			size_t pos = line.find("<ClInclude Include=");
			if (pos != string::npos) {

				out << "<ClCompile Include=\"" << "functions_AddToProject.h\" />\n";
				flagH = true;
			}
		}
	}
	in.close();
	out.close();

	CString fin(fname.c_str());
	DeleteFile(fin);

	rename(ftemp.c_str(), fname.c_str());

	fname += ".filters";
	in.open(fname.c_str());
	out.open(ftemp);

	flagCPP = false;
	flagH = false;
	/*
	<ClCompile Include="Complex.cpp">
	<Filter>Source Files</Filter>
	</ClCompile> */

	while (getline(in, line))
	{
		out << line << '\n';

		if (!flagCPP) {
			/*
			<ClCompile Include="Complex.cpp">
			<Filter>Source Files</Filter>
			</ClCompile> */

			size_t pos = line.find("</ClCompile>");
			if (pos != string::npos) {

				out << "    <ClCompile Include=\"" << "functions_AddToProject.cpp\" />\n" <<
					"      <Filter>Source Files</Filter>\n" <<
					"    </ClCompile> \n";
				flagCPP = true;
			}
		}
		if (!flagH) {
			/*
			<ClInclude Include="targetver.h">
			<Filter>Header Files</Filter>
			</ClInclude>
			*/
			size_t pos = line.find("<ClInclude Include=");
			if (pos != string::npos) {

				out << "    <ClInclude Include=\"" << "functions_AddToProject.h\" />\n" <<
					"      <Filter>Source Files</Filter>\n" <<
					"    </ClInclude> \n";
				flagH = true;
			}
		}
	}
	in.close();
	out.close();

	CString ffin(fname.c_str());
	DeleteFile(ffin);

	rename(ftemp.c_str(), fname.c_str());

}


void createFilesForFunctions() {
	std::ofstream outH, outCPP;
	outH.open(fhfunctions);
	outCPP.open(ffunctions);
	outH << "#ifndef Functions_AddToProject_H\n#define Functions_AddToProject_H\n";
	outCPP << "#include \"stdafx.h\"\n#include \"functions_AddToProject.h\"\n";

	for (auto m : strings) {
		string body = bodies[m.first];
		size_t pos = body.find("{");

		body.replace(pos, 1, ";");
		body.resize(pos + 2, '$');


		outH << body + "\n";
		outCPP << bodies[m.first];
	}
	outH << "\n#endif";
	outH.close();
	outCPP.close();
}
void insertFunctionForString(string forigin) {
	std::ifstream in;

	in.open(forigin.c_str());

	std::ofstream out;
	out.open(ftemp);
	if (!in || !out)
	{
		TCHAR e[1000] = _T("Error");
		MessageBox(NULL, e, _T("Could not open file"), NULL);
		//cerr << "Could not open file \n";
		return;
	}

	string line;
	string wordBefore = "#include";
	bool isDone = false;
	while (getline(in, line))
	{
		size_t p = line.find("//", 0);

		//remove comment
		if (p != string::npos)
			line.resize(p, '$');

		out << line << '\n';


		if (!isDone) {
			size_t pos = line.find(wordBefore);
			if (pos != string::npos) {
				out << "#include \"functions_AddToProject.h\"" << "\r\n";
				isDone = true;
			}
		}

	}

	in.close();
	out.close();
	CString fin(forigin.c_str());
	DeleteFile(fin);

	rename(ftemp.c_str(), forigin.c_str());
}

string get3numbers(long n) {
	std::random_device rd; // obtain a random number from hardware
	std::mt19937 eng(rd()); // seed the generator
	std::uniform_int_distribution<> distr(20, 1000); // define the range


	long z1 = distr(eng);
	long z2 = distr(eng);

	// Calculate the difference
	long d1 = n - z1;

	// Calculate the difference
	long d2 = z1 - z2;

	//cout<<d1+d2+z2<<"="n<<<<endl;

	long op1 = labs(d1);
	long op2 = labs(d2);
	long op3 = labs(z2);
	char s[1024];

	if (d1 > 0 && d2 > 0 && z2 > 0)
		sprintf(s, "(0%lo + 0x%lx + 0%lo)", op1, op2, op3);
	else if (d1 > 0 && d2 > 0 && z2 <= 0)
		sprintf(s, "(0%lo + 0x%lx - 0x%lx)", op1, op2, op3);
	else if (d1 > 0 && d2 <= 0 && z2 <= 0)
		sprintf(s, "(0%lo - 0x%lx - 0%lo)", op1, op2, op3);
	else if (d1 <= 0 && d2 <= 0 && z2 <= 0)
		sprintf(s, "(-0%lo - 0x%lx - 0x%lx)", op1, op2, op3);
	else if (d1 <= 0 && d2 <= 0 && z2 > 0)
		sprintf(s, "(-0%lo - 0x%lx + 0%lo)", op1, op2, op3);
	else if (d1 > 0 && d2 <= 0 && z2 > 0)
		sprintf(s, "(0%lo - 0x%lx + 0x%lx)", op1, op2, op3);
	else if (d1 <= 0 && d2 > 0 && z2 <= 0)
		sprintf(s, "(-0%lo + 0x%lx - 0%lo)", op1, op2, op3);
	else if (d1 <= 0 && d2 > 0 && z2 > 0)
		sprintf(s, "(-0%lo + 0x%lx + 0x%lx)", op1, op2, op3);

	string res(s);
	return res;
}

void getNumbers(string fname) {
	std::ifstream in;
	in.open(fname.c_str());

	string line;


	//check number
	regex r("^[0-9]+$");
	smatch match;
	string s;


	while (getline(in, s))
	{
		size_t p = s.find("//", 0);

		//remove comment
		if (p != string::npos)
			s.resize(p, '$');
		stringstream ss;

		//Storing the whole string into string stream
		ss << s;

		//Running loop till the end of the stream *
		string temp;
		int found;
		while (!ss.eof()) {

			// extracting word by word from stream
			ss >> temp;

			// Checking the given word is integer or not
			if (stringstream(temp) >> found) {
				//cout << found << " ";

				numbers[to_string(abs(found))] = get3numbers(abs(found));//to_string(found);
			}

			// To save from space at the end of string
			temp = "";
		}

	}
}


void getTokens(string fname) {
	std::ifstream in;
	in.open(fname.c_str());

	string line;


	//check idetifier
	regex r("^[a-zA-Z_][a-zA-Z0-9_]*$");

	smatch match;
	string types[] = { "void","int","float","double","string",
		"wstring","char","wchar_t", "WCHAR","long","bool","BOOL","CString" };

	string ends[] = { ";",")", "[","," }, end1, end2 = "(", s;

	while (getline(in, s))
	{
		size_t p = s.find("//", 0);


		//remove comment
		if (p != string::npos)
			s.resize(p, '$');

		//skip virual functions
		p = s.find("virtual", 0);
		if (p != string::npos)
			continue;

		for (string sub : types) {
			size_t pos = s.find(sub, 0);
			while (pos != string::npos)
			{
				//search functions
				size_t pos2 = s.find(end2, pos + sub.size());
				if (pos2 != string::npos) {


					string iden = trim(s.substr(pos + sub.size(), pos2 - pos - sub.size()));
					regex_search(iden, match, r);
					if (match.size()) {
						//cout<<iden<<endl;
						functions[iden] = "f_" + to_string(hash<string>{}(iden));;
					}

				}
				//search identifiers
				for (string end1 : ends) {
					size_t pos1 = s.find(end1, pos + sub.size());
					if (pos1 != string::npos) {
						string possible = s.substr(pos + sub.size(), pos1 - pos - sub.size());
						string iden = trim(possible);
						regex_search(iden, match, r);
						if (match.size()) {
							//cout<<iden<<endl;
							idens[iden] = "v_" + to_string(hash<string>{}(iden));
							if (end1 == ";")
								goto m1;

						}
						size_t poss = pos1 + 1;
						size_t pos22 = s.find("[", poss);
						if (pos22 != string::npos) {
							possible = s.substr(poss, pos22 - poss);
							iden = trim(possible);
							regex_search(iden, match, r);
							if (match.size()) {
								//cout<<iden<<endl;
								idens[iden] = "v_" + to_string(hash<string>{}(iden));
							}
						}
						if (end1 == ",") {
							size_t poss = pos1 + 1;
							size_t pos2 = s.find(",", poss);
							while (pos2 != string::npos) {

								possible = s.substr(poss, pos2 - poss);
								iden = trim(possible);
								regex_search(iden, match, r);
								if (match.size()) {
									//cout<<iden<<endl;
									idens[iden] = "v_" + to_string(hash<string>{}(iden));

								}
								else {
									size_t pos22 = s.find("[", poss);
									if (pos22 != string::npos) {
										possible = s.substr(poss, pos22 - poss);
										iden = trim(possible);
										regex_search(iden, match, r);
										if (match.size()) {
											//cout<<iden<<endl;
											idens[iden] = "v_" + to_string(hash<string>{}(iden));
										}
									}

								}
								poss = pos2 + 1;
								pos2 = s.find(end1, poss);
							}//while ','


							pos1 = s.find(";", poss);
							if (pos1 != string::npos) {

								possible = s.substr(poss, pos1 - poss);
								iden = trim(possible);
								regex_search(iden, match, r);
								if (match.size()) {
									//cout<<iden<<endl;
									idens[iden] = "v_" + to_string(hash<string>{}(iden));
									goto m1;
								}
							}
							pos1 = s.find("[", poss);
							if (pos1 != string::npos) {

								possible = s.substr(poss, pos1 - poss);
								iden = trim(possible);
								regex_search(iden, match, r);
								if (match.size()) {
									//cout<<iden<<endl;
									idens[iden] = "v_" + to_string(hash<string>{}(iden));;
								}
							}


						}//if ','


					}//if pos



				}// for iden
			m1:

				pos = s.find(sub, pos + 1);
			} //while iden
		}//for type
	}//while getline
}


void getStrings(string fname) {
	std::ifstream in;
	in.open(fname.c_str());

	string line;
	while (getline(in, line))
	{
		size_t p = line.find("//", 0);

		//remove comment
		if (p != string::npos)
			line.resize(p, '$');

		p = line.find("#include", 0);
		if (p != string::npos)
			continue;
		p = line.find("#pragma", 0);
		if (p != string::npos)
			continue;

		p = line.find("#define", 0);
		if (p != string::npos)
			continue;

		std::vector<std::string> itms = split(line, '"');

		if (itms.size() > 0) {
			for (std::vector<std::string>::iterator it = itms.begin() + 1; it != itms.end(); it++) {

				//size_t pos = line.find("\%");
				//if (pos == string::npos && (*it).size()>0) {
				if ((*it).size() > 0) {
					string prev = *(it - 1);
					string s = *it;

					CString OriginalText(s.c_str());
					CString ResultText;
					int tSize = prev.size();

					if (tSize > 1 && prev[tSize - 1] == '\\')
						continue;

					if (tSize > 1 && prev[tSize - 1] == 'L') {
						ResultText = stringToObfuscate(OriginalText, true);
						//std::cout << "L " << *it << endl;
					}
					else {
						//std::cout << *it << endl;
						ResultText = stringToObfuscate(OriginalText, false);
					}

					string sss = "\"" + *it + "\"";

					strings[sss] = "s_" + to_string(hash<string>{}(*it)) + "()";
					bodies[sss] = CT2A(ResultText.GetString());
				}
				it++;
				if (it == itms.end())
					break;
			}
		}
	}

}




string print(map<string, string> pairs) {
	string s = "";
	for (auto elem : pairs)
	{
		//std::cout << elem.first << "\t->\t" << elem.second << "\n";
		s += elem.first + "\t->\t" + elem.second + "\r\n";
	}
	return s;
}





int RandomIntFromRange(int From, int To)
{
	int result;
	std::random_device rd;
	pcg rand(rd);
	std::uniform_int_distribution<> u(From, To);
	result = u(rand);
	return result;
}

bool find(const std::vector<int>& vec, int index)
{
	for (int i = 0; i < vec.size(); ++i)
	{
		if (index == vec[i])
			return true;
	}

	return false;
}

void shuffle(int array[], const int size)
{
	const int n_size = size;
	int temp[1028];
	std::vector<int> indices;

	for (int i(0); i < size; ++i)
		temp[i] = array[i];

	int index = rand() % size;
	indices.push_back(index);

	for (int i = 0; i < size; ++i)
	{
		if (i == 0)
			array[i] = temp[index];
		else
		{
			while (find(indices, index))
				index = rand() % size;

			indices.push_back(index);
			array[i] = temp[index];
		}
	}

}

CString stringToObfuscate(CString OriginalText, BOOL IsUNICODEChecked) {
	CString ResultText, ResultTextLine, VarnameText;
	int i;
	int d;
	int z;
	int x;
	int choice;

	string source = CT2A(OriginalText.GetString());

	string r = "_";
	r += to_string(hash<string>{}(source));
	CString cstr(r.c_str());
	VarnameText = cstr;


	WCHAR c;
	CString Formula;
	CString TextWithJunk;
	TextWithJunk = OriginalText;
	int Length = OriginalText.GetLength();
	TextWithJunk += (CString)L" ";
	for (i = Length + 1; i < Length * 2; i++)
	{
		WCHAR result = RANDOM_WCHAR;
		TextWithJunk += (CString)(result);
	}



	if (IsUNICODEChecked) // UNICODE
	{
		ResultTextLine.Format(L"\nwchar_t* s%s(){ ", VarnameText);
		ResultText = ResultText + ResultTextLine + L"\r\n";
		ResultTextLine.Format(L"wchar_t* %s=new wchar_t[%d];", VarnameText, Length * 2);
	}
	else
	{
		ResultTextLine.Format(L"\nchar* s%s(){ ", VarnameText);
		ResultText = ResultText + ResultTextLine + L"\r\n";
		ResultTextLine.Format(L"char* %s=new char[%d];", VarnameText, Length * 2);
	}


	ResultText = ResultText + ResultTextLine + L"\r\n";

	TextWithJunk.SetAt(Length, L'\0');
	Length = Length * 2;
	for (i = 0; i < Length; i++) arr[i] = i;
	shuffle(arr, Length);
	for (i = 0; i < Length; i++)
	{
		c = TextWithJunk[arr[i]];
		// Select obfuscation option for this character
		choice = RANDOM_DIGIT;
		x = TextWithJunk[arr[i]];

		// Calculate Z
		z = RANDOM_CODE;
		if (z == 96)
			z = 95;
		if (z == 92)
			z = 93;

		// Calculate the difference
		d = z - x;

		switch (choice)
		{
		case 10:
		case 1:
			if (d > 0) {
				Formula.Format(L"0x%x - 0%o", z, d);
			}
			else {
				Formula.Format(L"0%o + 0x%x", z, -d);
			}
			break;
		case 2:
		case 3:
			if (d > 0) {
				Formula.Format(L"0x%x - 0%o", z, d);
			}
			else {
				Formula.Format(L"0%o + 0x%x", z, -d);
			}

			break;
		case 4:
		case 5:
			if (d > 0) {

				if (IsUNICODEChecked) // UNICODE
					Formula.Format(L"L'%c' - 0x%x", z, d);
				else
					Formula.Format(L"'%c' - 0%o", z, d);
			}
			else {
				if (IsUNICODEChecked) // UNICODE
					Formula.Format(L"L'%c' + 0%o", z, -d);
				else
					Formula.Format(L"'%c' + 0x%x", z, -d);
			}

			break;
		case 6:
		case 7:


			if (d > 0) {

				if (IsUNICODEChecked) // UNICODE
					Formula.Format(L"L'%c' - 0%o", z, d);
				else
					Formula.Format(L"'%c' - 0%o", z, d);
			}
			else {
				if (IsUNICODEChecked) // UNICODE
					Formula.Format(L"L'%c' + 0%o", z, -d);
				else
					Formula.Format(L"'%c' + 0%o", z, -d);
			}
			break;
		case 8:
		case 9:
			// x = 'z'
			Formula.Format(L"%d", x);
			break;
		}
		ResultTextLine.Format(L" %s[0x%x] = %s;", VarnameText, arr[i], Formula);
		int ccc = ResultTextLine.Find(L'\\');
		if (ccc > -1)
		{
			ResultTextLine = ResultTextLine.Left(ccc) + L"\\" + ResultTextLine.Mid(ccc);
		}

		ResultText = ResultText + ResultTextLine + L"\r\n";

	}
	ResultTextLine.Format(L" %s[0x%x] = \'\\0\';", VarnameText, Length - 1);
	ResultText = ResultText + ResultTextLine;
	ResultTextLine.Format(L"\r\nreturn %s;\r\n}\r\n", VarnameText);
	ResultText = ResultText + ResultTextLine;
	return ResultText;
}


////////////////////////////////////////////////////
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

														// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTinyObfuscateDlg dialog



CTinyObfuscateDlg::CTinyObfuscateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTinyObfuscateDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTinyObfuscateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, Original);
	DDX_Control(pDX, IDC_EDIT1, Result);
	DDX_Control(pDX, IDC_EDIT3, Varname);
	DDX_Control(pDX, IDC_CHECK_UNICODE, m_Unicode);
	DDX_Control(pDX, IDC_CHECK_UNICODE3, m_Function);
	DDX_Control(pDX, IDC_CHECK_UNICODE2, m_Variable);
	DDX_Control(pDX, IDC_CHECK_UNICODE4, m_Number);
	DDX_Control(pDX, IDC_CHECK_UNICODE5, m_String);

	DDX_Control(pDX, IDC_EDIT4, m_ProjectFileName);
	DDX_Control(pDX, IDC_LIST_FILES, m_ProjectFiles);
}

BEGIN_MESSAGE_MAP(CTinyObfuscateDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTinyObfuscateDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CTinyObfuscateDlg::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT3, &CTinyObfuscateDlg::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON2, &CTinyObfuscateDlg::OnBnClicked_ScanProjectFile)
	ON_BN_CLICKED(IDC_BUTTON1, &CTinyObfuscateDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_ALL, &CTinyObfuscateDlg::OnBnClicked_AllFiles)

END_MESSAGE_MAP()


// CTinyObfuscateDlg message handlers

BOOL CTinyObfuscateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);            // Set big icon
	SetIcon(m_hIcon, FALSE);        // Set small icon

									// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTinyObfuscateDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTinyObfuscateDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTinyObfuscateDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTinyObfuscateDlg::OnBnClickedOk()
{
	CString OriginalText, ResultText, ResultTextLine, VarnameText;
	Original.GetWindowTextW(OriginalText);
	Varname.GetWindowTextW(VarnameText);

	string source = CT2A(OriginalText.GetString());
	if (source.size() > 0) {
		string r = "s_";
		r += to_string(hash<string>{}(source)) + "()";
		CString cstr(r.c_str());
		VarnameText = cstr;
		Varname.SetWindowTextW(VarnameText);

		// Check if we need a UNICODE string or not
		BOOL IsUNICODEChecked;
		IsUNICODEChecked = (m_Unicode.GetCheck() == 1);

		ResultText = stringToObfuscate(OriginalText, IsUNICODEChecked);
		Result.SetWindowTextW(ResultText);


	}
}


void CTinyObfuscateDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CTinyObfuscateDlg::OnEnChangeEdit3()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CTinyObfuscateDlg::OnBnClicked_ScanProjectFile()
{
	CStringArray result;
	static wchar_t drive[_MAX_DRIVE];
	static wchar_t somepath[_MAX_PATH];
	static wchar_t buffer[_MAX_PATH];
	static wchar_t path[_MAX_PATH];
	static wchar_t ext[_MAX_EXT];

	CString projectName;
	m_ProjectFileName.GetWindowText(projectName);
	_wsplitpath_s(projectName.GetBuffer(), drive, somepath, buffer, ext);

	FILE * f;
	_wfopen_s(&f, projectName, L"r");
	if (f == NULL)
	{
		wprintf(L"ERROR: Project %s is missing or unavailable.\n", projectName);
		exit(1);
	}

	while (!feof(f))
	{
		wchar_t *res = fgetws(buffer, sizeof(buffer) * sizeof(wchar_t), f);
		if (res == NULL)
			continue;

		if ((wcsstr(buffer, L"<ClInclude Include="))
			|| (wcsstr(buffer, L"<ClCompile Include=")))
		{
			wchar_t * ptrName = wcschr(buffer, L'=');
			wchar_t * ptrName1 = wcsstr(buffer, L"/>");
			if (ptrName1) *ptrName1 = 0;

			while (('=' == *ptrName)
				|| (' ' == *ptrName)
				|| ('"' == *ptrName)) ptrName++;
			while (('"' == ptrName[wcslen(ptrName) - 1])
				|| (' ' == ptrName[wcslen(ptrName) - 1])
				|| ('\n' == ptrName[wcslen(ptrName) - 1]))
				ptrName[wcslen(ptrName) - 1] = 0;

			_wmakepath_s(path, drive, somepath, ptrName, NULL);
			result.Add(path);

		}

	}


	CString myFile = somepath;



	string ff = CT2A(myFile.GetString());
	ftemp = ff + "temp.txt";
	ffunctions = ff + "functions_AddToProject.cpp";
	fhfunctions = ff + "functions_AddToProject.h";
	string fpr = CT2A(projectName.GetString());

	fproject = fpr;

	fileH.clear();
	fileCPP.clear();
	functions.clear();
	numbers.clear();
	idens.clear();
	strings.clear();
	bodies.clear();

	m_ProjectFiles.ResetContent();

	//create *.cpp.old file
	for (int i = 0; i < result.GetCount(); i++)
	{
		CString myFile = result[i];


		//create *.cpp.old file
		string source = CT2A(myFile.GetString());
		if (existsFile(source + ".old")) {
			AfxMessageBox(_T("source code already obfuscate"));
			return;
		}

		size_t pos1 = source.find("stdafx");
		size_t pos2 = source.find("esource");///resource, Resource
		size_t pos3 = source.find("targetver");

		if (pos1 == string::npos &&
			pos2 == string::npos &&
			pos3 == string::npos) {

			string old = source + ".old";
			copyFile(source, old);

			wprintf(L"%d %s\n", i, myFile);
			m_ProjectFiles.AddString(myFile);

			if (source[source.size() - 1] == 'h') {//*.h
				fileH.push_back(source);
			}
			else {//*.cpp
				fileCPP.push_back(source);
				getStrings(source);

			}

			getTokens(source);
			getNumbers(source);
		}

	}
	functions.erase("main");
	functions.erase("_tmain");
	functions.erase("WinMain");
	functions.erase("CWinMain");
	functions.erase("_tWinMain");
	functions.erase("operator");
	fclose(f);

	return;
}



void CTinyObfuscateDlg::OnBnClickedButton1()
{
	const TCHAR szFilter[] = _T("Project Files (*.vcxproj)|*.vcxproj|All Files (*.*)|*.*||");
	CFileDialog dlg(TRUE, _T("vcxproj"), NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, this);
	if (dlg.DoModal() == IDOK)
	{
		CString sFilePath = dlg.GetPathName();
		m_ProjectFileName.SetWindowText(sFilePath);
	}
}

void CTinyObfuscateDlg::OnBnClicked_AllFiles()
{

	// Check if we need to replace function name or not
	BOOL IsFunctionChecked;
	IsFunctionChecked = (m_Function.GetCheck() == 1);

	// Check if we need to replace variable name or not
	BOOL IsVariableChecked;
	IsVariableChecked = (m_Variable.GetCheck() == 1);

	// Check if we need to replace number or not
	BOOL IsNumberChecked;
	IsNumberChecked = (m_Number.GetCheck() == 1);

	// Check if we need to replace string literal or not
	BOOL IsStringChecked;
	IsStringChecked = (m_String.GetCheck() == 1);
	//AfxMessageBox(_T("Please wait..."));

	Original.SetWindowTextW(L"");
	Varname.SetWindowTextW(L"");

	CString show1(print(numbers).c_str());
	CString ResultText1 = L"NUMBERS CONVERSION\r\n" + show1 + L"\r\n\r\n";

	CString show2(print(strings).c_str());
	CString ResultText2 = L"STRINGS CONVERSION\r\n" + show2 + L"\r\n\r\n";

	CString show3(print(idens).c_str());
	CString ResultText3 = L"VARIABLES CONVERSION\r\n" + show3 + L"\r\n\r\n";

	CString show4(print(functions).c_str());
	CString ResultText4 = L"FUNCTIONS CONVERSION\r\n" + show4 + L"\r\n\r\n";


	Result.SetWindowTextW(ResultText1 + ResultText2 + ResultText3 + ResultText4);

	if (IsStringChecked) {
		createFilesForFunctions();
		AddExistingFilesToProject();
	}

	for (string f : fileCPP) {
		if (IsStringChecked) {
			insertFunctionForString(f);
			replaceStrings(f);
		}
		if (IsVariableChecked)
			replaceVariables(f);
		if (IsFunctionChecked)
			replaceFunctions(f);
		if (IsNumberChecked)
			replaceNumbers(f);


	}
	for (string f : fileH) {
		if (IsVariableChecked)
			replaceVariables(f);
		if (IsFunctionChecked)
			replaceFunctions(f);
		if (IsNumberChecked)
			replaceNumbers(f);
	}

	//AfxMessageBox(_T("Done!\nWarning: name conflicts may occur whithin third-party libraries and classes!"));

}

