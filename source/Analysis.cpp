#include "Analysis.h"

//�R���X�g���N�^
Analysis::Analysis()
{

}

//�f�X�g���N�^
Analysis::~Analysis()
{

}

void Analysis::MakeVelFile(const std::string& filename)
{

}

// velocity.csv��ǂݍ��݁A���ϑ��x���v�Z����
double Analysis::CalcVelAve(const std::string& filePath)
{
	std::ifstream file(filePath);
	std::string line;
	double xSum = 0.0, ySum = 0.0;
	size_t count = 0;

	// �w�b�_�[�s���X�L�b�v
	std::getline(file, line);

	// �f�[�^�s������
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string time, xStr, yStr;

		std::getline(ss, time, ',');
		std::getline(ss, xStr, ',');
		std::getline(ss, yStr, ',');

		xSum += std::stod(xStr);
		ySum += std::stod(yStr);
		++count;
	}

	double xAve = xSum / count;
	double yAve = ySum / count;

	return sqrt(xAve * xAve + yAve * yAve);
}

std::pair<std::string, std::string> Analysis::GetVarValues(const std::string& path)
{
	std::regex varRegex(R"(.*lorentzForce=([^_]+)_siteDistance=([^/]+).*)");
	std::smatch match;

	if (std::regex_match(path, match, varRegex) && match.size() == 3) {
		return { match[1].str(), match[2].str() };
	}

	return { "unknown", "unknown" };
}

void Analysis::WriteResult(const std::string& filename)
{
	std::ofstream file(filename);
	file << "lorentzForce,siteDistance,vAve\n";

}