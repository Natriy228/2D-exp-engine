#include <string>
#include <vector>
#ifndef _GF_FF_

std::vector <std::string> SlowSplit(std::string st, std::string ch) {
	std::vector <std::string> res;
	std::string s;
	s.clear();
	for (int i = 0; i < st.size(); i++) {
		bool trg = true;
		for (int b = i; b < i + ch.size(); b++) {
			if (b < st.size()) {
				if (st[b] != ch[b - i]) {
					trg = false;
				}
			}
		}
		if (trg or i == st.size() - 1) {
			if (i == st.size() - 1) {
				s += st[i];
			}
			res.push_back(s);
			s.clear();
			i += ch.size() - 1;
		}
		else {
			s += st[i];
		}
	}
	return res;
}

std::vector <std::string> Split(std::string st) {
	std::vector <std::string> res;
	std::string s;
	s.clear();
	for (char i : st) {
		if (i == ' ') {
			res.push_back(s);
			s.clear();
		}
		else {
			s += i;
		}
	}
	res.push_back(s);
	return res;
}
#endif