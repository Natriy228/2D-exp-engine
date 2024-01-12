#include "EngineWork.h"

class Charge : public Object {
private:
	double X = 0, Y = 0;
	double rotation = 0, scale = 1;
	Vector center;
	std::string type = "phys";
	std::vector <Mesh> meshes;
	std::vector <std::vector <Vector>> ready_points;
	std::vector <std::string> tags;

	Vector v = Vector(0, 0);
	double q = 0;
	double m = 10000000000;
	int active = 1;
public:
	void init(Mesh m, int fm, int x, int y, int z, float rot, float sc, std::string tgs, std::string arg, std::string tp) override {
		this->X = x;
		this->Y = y;
		this->layer = z;
		this->FocusMesh = fm;
		this->meshes.push_back(m);
		this->rotation = rot;
		this->scale = sc;
		this->tags = Split(tgs);
		this->type = tp;
		this->center = this->meshes[0].Cpoint;
		this->ready_points = RecountPoints(this->meshes, this->X, this->Y, this->rotation, this->scale, this->center);
	}

	Vector GetLocation() override {
		return Vector(this->X, this->Y);
	}

	std::vector <Vector> GetPoints(int i) override {
		return ready_points[i];
	}

	int GetMeshesCount() override {
		return meshes.size();
	}

	Mesh *GetMesh(int i) override {
		return &meshes[i];
	}

	void ChangeLocation(double x = 0, double y = 0, bool offset = false) override {
		if (offset) {
			X += x;
			Y += y;
		}
		else {
			X = x;
			Y = y;
		}
		ready_points = RecountPoints(this->meshes, this->X, this->Y, this->rotation, this->scale, this->center);
	}

	void GiveQ() {
		ObjectPointer gh;
		gh = this;
		for (ObjectPointer i : ActiveWorld.objects) {
			if (Collision(i, gh).collision and i.get() != this) {
				double pq = std::stod(i.get()->update(0, "getq", ""));
				if (pq > q) {
					double nq = (pq + q) / 2;
					q = nq;
					i.get()->update(0, "setq", std::to_string(nq));
				}
			}
		}
	}

	std::string update(double delta, std::string task = "", std::string arguments = "") override {
		if (task == "setq") {
			q = std::stod(arguments);
			return "";
		}
		else if (task == "getq") {
			return std::to_string(q);
		}
		else if (task == "toggle") {
			active = (active + 1) % 2;
			return "";
		}
		else {
			for (auto i : ActiveWorld.objects) {
				if (i.get() != this) {
					double pq = std::stod(i.get()->update(0, "getq", ""));
					Vector r = normalise(CreateVector(X, Y, i.get()->GetLocation().X, i.get()->GetLocation().Y));
					double rl = VectorLengh(CreateVector(X, Y, i.get()->GetLocation().X, i.get()->GetLocation().Y));
					Vector a;
					if (rl != 0) {
						a = Vector((pq * q * 9 * pow(10, 9) / rl * rl * r.X) / m, (pq * q * 9 * pow(10, 9) / rl * rl * r.Y) / m);
					}
					else {
						a = (0, 0);
					}
					v.X += a.X * delta * -1 * active;
					v.Y += a.Y * delta * -1 * active;
				}
			}
			ObjectPointer gh;
			gh = this;
			this->ChangeLocation(v.X * delta * active, 0, true);
			GiveQ();
			for (ObjectPointer i : ActiveWorld.objects) {
				if (Collision(gh, i).collision and i.get() != this) {
					this->ChangeLocation(-1 * v.X * delta * active, 0, true);
					v.X = 0;
					break;
				}
			}

			this->ChangeLocation(0, v.Y * delta * active, true);
			GiveQ();
			for (ObjectPointer i : ActiveWorld.objects) {
				if (Collision(gh, i).collision and i.get() != this) {
					this->ChangeLocation(0, -1 * v.Y * delta * active, true);
					v.Y = 0;
					break;
				}
			}

			float c = q / 255;
			if (c > 0) {
				meshes[0].ChangeColor(0, 0, 255, 255);
			}
			else if (c == 0) {
				meshes[0].ChangeColor(255, 255, 255, 255);
			}
			else {
				meshes[0].ChangeColor(255, 0, 0, 255);
			}
			return "";
		}
	}
};


class Character : public Object {
private:
	double X = 0, Y = 0;
	double rotation = 0, scale = 1;
	Vector center;
	std::string type = "phys";
	std::vector <Mesh> meshes;
	std::vector <std::vector <Vector>> ready_points;
	std::vector <std::string> tags;
public:
	double yv = 0, xv = 0, a = 160;
	int xaxis = 0, yaxis = 0;

	std::string update(double delta, std::string task = "", std::string arguments = "") override {
		bool lb = false, rb = false, fb = false, bb = false;
		ObjectPointer gh;
		gh = this;
		if (task == "control") {
			auto buf = Split(arguments);
			xaxis = std::stoi(buf[0]);
			yaxis = std::stoi(buf[1]);
		}
		else {
			if (yv <= 200) {
				yv += a * delta;
			}
			this->ChangeLocation(0, yv * delta, true);
			for (ObjectPointer i : ActiveWorld.objects) {
				if (BoxCollision(gh, i) and i.get() != this) {
					this->ChangeLocation(0, yv * delta * -1, true);
					if (yv >= 0) {
						bb = true;
					}
					else {
						fb = true;
					}
					break;
				}
			}
			if (bb == false) {
				xv += xaxis * 1;
			}
			else {
				xv += xaxis * 160;
			}
			if (xv > 160) {
				xv = 160;
			}
			if (xv < -160) {
				xv = -160;
			}
			this->ChangeLocation(xv * delta, 0, true);
			for (ObjectPointer i : ActiveWorld.objects) {
				if (BoxCollision(gh, i) and i.get() != this) {
					this->ChangeLocation(xv * delta * -1, 0, true);
					if (xv >= 0) {
						rb = true;
					}
					else {
						lb = true;
					}
					break;
				}
			}
			if (bb) {
				yv = 0;
			}
			if (bb and xaxis == 0) {
				if (xv < 0) {
					xv += 20;
				}
				if (xv > 0) {
					xv -= 20;
				}
				if (xv < 20 or xv > -20) {
					xv = 0;
				}
			}
			if (lb or rb) {
				xv = 0;
			}
			if (yaxis == -1 and bb == true) {
				yv = -160;
			}
			if (yaxis == -1 and (lb or rb)) {
				yv = -120;
			}
			if (fb) {
				yv = 0;
			}
		}
		return "";
	}

	void init(Mesh m, int fm, int x, int y, int z, float rot, float sc, std::string tgs, std::string arg, std::string tp) override {
		this->X = x;
		this->Y = y;
		this->layer = z;
		this->FocusMesh = fm;
		this->meshes.push_back(m);
		this->rotation = rot;
		this->scale = sc;
		this->tags = Split(tgs);
		this->type = tp;
		this->center = this->meshes[0].Cpoint;
		this->ready_points = RecountPoints(this->meshes, this->X, this->Y, this->rotation, this->scale, this->center);
	}

	Vector GetLocation() override {
		return Vector(this->X, this->Y);
	}

	std::vector <Vector> GetPoints(int i) override {
		return ready_points[i];
	}

	int GetMeshesCount() override {
		return meshes.size();
	}

	Mesh* GetMesh(int i) override {
		return &meshes[i];
	}

	void AddMesh(Mesh m) {
		this->meshes.push_back(m);
	}

	void ChangeLocation(double x = 0, double y = 0, bool offset = false) override {
		if (offset) {
			X += x;
			Y += y;
		}
		else {
			X = x;
			Y = y;
		}
		ready_points = RecountPoints(this->meshes, this->X, this->Y, this->rotation, this->scale, this->center);
	}
};

void ChangeWorld(std::string FileName = "") {
	World neww;
	std::ifstream streamer;
	streamer.open(FileName);
	std::string a;

	std::getline(streamer, a);
	if (a == "#PTPWorld#") {
		while (a != "end") {
			std::getline(streamer, a);
			auto b = Split(a);
			if (b[0] == "Object") {
				ObjectPointer gh;
				gh = neww.spawn<Object>(ReadMesh(b[1]), stod(b[2]), stod(b[3]), stod(b[4]), stod(b[5]), b[6], b[7]);
				std::string cmd = b[8];
				std::string arg = " ";
				int c = 8;
				while (b[c] != "end") {
					arg.clear();
					cmd = b[c];
					while (b[c] != "/" and b[c] != "end") {
						c++;
						arg += b[c] + " ";
					}
					gh.get()->update(0, cmd, arg);
				}
			}
			else if (b[0] == "Charge") {
				neww.spawn<Charge>(ReadMesh(b[1]), stod(b[2]), stod(b[3]), stod(b[4]), stod(b[5]), b[6], b[7]);
			}
			else if (b[0] == "Character") {
				neww.spawn<Character>(ReadMesh(b[1]), stod(b[2]), stod(b[3]), stod(b[4]), stod(b[5]), b[6], b[7]);
			}
			else {
				e_log.write("Для класса " + b[0] + " нет декодера.");
			}
		}
	}
	ActiveWorld = neww;
}