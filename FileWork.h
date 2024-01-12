#ifndef _LOG_H_
#include <string>
#include <fstream>
#include <ctime>

class E_log {
private:
	std::string dir;
public:
	E_log(std::string directory = "") {
		dir = directory;
		std::ofstream logfile;
		logfile.open(dir);
		logfile << "Journal\n";
		logfile.close();
	}
	void write(std::string message = "") {
		std::ofstream logfile;
		logfile.open(dir, std::ios_base::out | std::ios_base::app);
		logfile << message + " " + "\n";
		logfile.close();
	}
};

#endif
