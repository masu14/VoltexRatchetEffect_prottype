#include "Analysis.h"

//�R���X�g���N�^
Analysis::Analysis()
{

}

//�f�X�g���N�^
Analysis::~Analysis()
{

}

void Analysis::MakeVelFile(const std::string& dirName)
{
	std::string fileName = dirName + "/velocity_averages.csv";

	std::vector<std::tuple<std::string, std::string, double>> results;

	for (const auto& entry : std::filesystem::recursive_directory_iterator(dirName)) {
		if (entry.is_regular_file() && entry.path().filename() == "velocity.csv") {
			//var1,var2�𒊏o
			auto [var1, var2] = GetVarValues(entry.path().parent_path().string());

			//velocity.csv�̕��ς��v�Z
			double vAve = CalcVelAve(entry.path().string());

			results.emplace_back(var1, var2, vAve);

		}
	}

	//���ʂ�csv�ɏ����o��
	WriteResult(fileName, results);
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

	return xAve;
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

void Analysis::WriteResult(const std::string& filename,
	const std::vector<std::tuple<std::string, std::string, double>>& results)
{
	std::ofstream file(filename);
	file << "lorentzForce,siteDistance,vAve\n";

	for (const auto& [var1, var2, vAve] : results) {
		file << var1 << "," << var2 << "," << vAve << "\n";
	}

}