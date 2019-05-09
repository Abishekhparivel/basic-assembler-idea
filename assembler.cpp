#include<bits/stdc++.h>
using namespace std;

ofstream fout;

string binary(int n)
{
	string s= "";
	for(;n>0;n/=2)
		s.push_back(n%2 + 48);
	while(s.length()<5)
	{
		s = s + "0";
	}
	reverse(s.begin(),s.end());
	return s;
}

string reg3l0(string s1, int errorCount, int lineNo)
{
	string ans="";
	bool regExist = false,regError = false;
	int i,n,regNo;
	for(i=0,n=0;n<3&&i<s1.length();i++,n++)
	{
		regNo = 0;
		if(s1[i]=='$')
		{
			i++;
			while(i<s1.length(),isdigit(s1[i]))
			{
				regExist = true;
				regNo = regNo*10 + s1[i]-48;
				i++;
			}
			if(regExist==true && regNo<32 && regNo>=0)
			{
				regExist = false;
				ans+=binary(regNo);
			}
			else
			{
				regError = true;
				fout<<"Invalid Register Number in line "<<lineNo<<endl;
				errorCount++;
				break;
			}
			if(s1[i]!=',' && i<n-1)
			{
				regError = true;
				fout<<"required ',' in line "<<lineNo<<endl;
				errorCount++;
				break;
			}
		}
		else
		{
			fout<<"Required a register in line "<<lineNo<<endl;
			errorCount++;
			regError = true;
			break;
		}
	}
	if(regError = false && n!=3)
	{
		fout<<"Required more no of registers in line "<<lineNo<<endl;
		errorCount++;
	}
	if(regError)
		return "";
	return ans;
}

string reg2l0(string s1, int errorCount, int lineNo)
{
	bool regError = false,regExist = false;
	int i=0,regNo=0;
	string ans="";
	if(s1[i]=='$')
	{
		i++;
		while(i<s1.length(),isdigit(s1[i]))
		{
			regExist = true;
			regNo = regNo*10 + s1[i]-48;
			i++;
		}
		if(regExist==true && regNo<32 && regNo>=0)
		{
			regExist = false;
			ans+=binary(regNo);
		}
		else
		{
			regError = true;
			fout<<"Inavlid Register Number in line "<<lineNo<<endl;
			errorCount++;
			return "";
		}
		if(i==s1.length() || (i<s1.length()&&s1[i++] != ','))
		{
			fout<<"required ',' in line "<<lineNo<<endl;
			errorCount++;
			return "";
		}
		while(i<s1.length()&&s1[i]!='(')
		{
			i++;
		}
		if(i==s1.length())
		{
			fout<<"required memory address in line "<<lineNo<<endl;
			errorCount++;
			return "";
		}
		regNo = 0;
		i++;
		if(s1[i]=='$')
		{
			i++;
			while(i<s1.length(),isdigit(s1[i]))
			{
				regExist = true;
				regNo = regNo*10 + s1[i]-48;
				i++;
			}
			if(regExist==true && regNo<32 && regNo>=0)
			{
				regExist = false;
				ans+=binary(regNo);
			}
			else
			{
				regError = true;
				fout<<"Inavlid Register No in line "<<lineNo<<endl;
				errorCount++;
				return "";
			}
			if(i==s1.length() || s1[i]!=')')
			{
				fout<<"error not found in line "<<lineNo<<endl;
				errorCount++;
				return "";
			}
		}
		else
		{
			fout<<"required a integer "<<lineNo<<endl;
			errorCount++;
			return "";
		}
	}
	else
	{
		fout<<"needed a register in line "<<lineNo<<endl;
		errorCount++;
		return "";
	}
	return ans + "00010";
}

int main()
{
	int ErrorCount=0;
	ifstream fin;
	fin.open("./input.txt");
	string code[100];
	int noOfLines = 0;
	while(fin)
	{
		string line, temp;
		getline(fin, line);
		int i1, flag = 1;
		if(line == "")
			continue;
		if(line[0] == ' ')
		{
			flag = 0;
			for(i1=0; line[i1]==' '; i1++)
			{}
			for(int j=0; i1<line.length(); j++, i1++)
				temp.push_back(line[i1]);
		}
		if(flag)
			temp = line;
		line = "";
		for(i1=0; i1<temp.length()&&temp[i1]!=' '; i1++)
			line.push_back(temp[i1]);
		line.push_back(' ');
		for(; i1<temp.length(); i1++)
		{
			while(temp[i1] == ' ')
				i1++;
			if(i1 < temp.length())
				line.push_back(temp[i1]);
		}
		int summa = line.length()-1;
		while(summa > 0 && line[summa] == ' ')
		{
			line.resize(summa);
			summa--;
		}
		code[noOfLines] = line;
		noOfLines++;
	}
	
	map<string, string> opCode;
	opCode["add"] = "000001";
	opCode["sub"] = "000010";
	opCode["mul"] = "000011";
	opCode["and"] = "000100";
	opCode["or"] = "000101";
	opCode["xor"] = "000110";
	opCode["not"] = "000111";
	opCode["nor"] = "001000";
	opCode["j"] = "001001";
	opCode["jal"] = "001010";
	opCode["jr"] = "001100";
	opCode["load"] = "100001";
	opCode["store"] = "100010";
	
	map<string, pair<int, int> > info;
	info["add"] = make_pair(3, 0);
	info["sub"] = make_pair(3, 0);
	info["mul"] = make_pair(3, 0);
	info["and"] = make_pair(3, 0);
	info["or"] = make_pair(3, 0);
	info["xor"] = make_pair(3, 0);
	info["nor"] = make_pair(3, 0);
	info["j"] = make_pair(0, 1);
	info["jal"] = make_pair(0, 1);
	info["jr"] = make_pair(1, 0);
	info["load"] = make_pair(2, 0);
	info["store"] = make_pair(2, 0);
	
	string ins[noOfLines];
	for(int i=0; i<noOfLines; i++)
		ins[i] = "";
	int insCount = 0;
	set<string> labels;
	fout.open("./output.txt");
	
	map<string, pair<int, int> > m;
	string str[100];
	int noOfLabels = 0;
	for(int i=0; i<noOfLines; i++)
	{
		if(code[i][code[i].length()-1] == ':')
		{
			str[noOfLabels] = code[i];
			noOfLabels++;
			if(m[code[i]].first == 0 && m[code[i]].second == 0)
			{
				string LabName = code[i];
				int labStart = 0, labEnd = 0;
				i++;
				if(code[i].compare("{") == 0)
				{
					labStart = i;
					i++;
					while(i<noOfLines)
					{
						if(code[i][0] == '}')
							break;
						else if(code[i][code[i].length()-1] == ':')
						{
							fout<<"Multiple labels in line "<<i<<endl;
							ErrorCount++;
						}
						else
						{
							string temp = "";
							bool insWo = false;
							int s;
							for(s=0; s<code[i].length(); s++)
							{
								if(code[i][s] == ' ')
								{
									insWo = true;
									break;
								}
								else
								{
									temp.push_back(code[i][s]);
								}
							}
							if(insWo == true)
							{
								string ans, s11="";
								for(s=s+1;s<code[i].length();s++)
									s11.push_back(code[i][s]);
								if(opCode[temp] != "")
								{
									ins[i] = opCode[temp];
									pair<int,int> p = info[temp];
									if(p.first == 3 && p.second == 0)
									{
										ans = reg3l0(s11, ErrorCount, i);
									}
									else if(p.first == 2 && p.second == 0)
									{
										ans = reg2l0(s11, ErrorCount, i);
									}
									else
									{
										ans = "010010001100001";
									}
									if(ans == "")
									{
										ErrorCount++;
									}
									ins[i] += ans;
								}
								else
								{
									fout<<"Inst not found in line "<<i<<endl;
									ErrorCount++;
								}
							}
							else
							{
								fout<<"No Instruction in line "<<i<<endl;
								ErrorCount++;
							}
						}
						i++;
					}
					labEnd = i;
					m[LabName] = make_pair(labStart, labEnd);
				}
				else
				{
					fout<<"required '{' in line "<<i<<endl;
					ErrorCount++;
				}
			}
			else
			{
				fout<<"Multi Label Dec in line "<<i<<endl;
				while(i < noOfLines && code[i].compare("}") == 0)
					i++;
				ErrorCount++;
			}
		}
		else
		{
			fout<<"No Label Name in line "<<i<<"\n";
			ErrorCount++;
		}
	}
	
	cout<<"No of Errors: "<<ErrorCount<<endl;
	if(!ErrorCount)
	{
		cout<<"Run successfull!!!"<<endl;
		for(int i=0; i<noOfLabels; i++)
		{
			for(int j=m[str[i]].first+1; j<m[str[i]].second; j++)
				fout<<ins[j]<<endl;
		}
	}
	return 0;
}
