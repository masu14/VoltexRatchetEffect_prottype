#include "FileHandler.h"


//�R���X�g���N�^
FileHandler::FileHandler()
	:fileName("")
{ /* DO_NOTHING */ }

//�f�X�g���N�^
FileHandler::~FileHandler()
{
	if (file.is_open()) {
		file.close();
	}
}

//----------------------------------------------------------------------------------------------
//    �f�B���N�g���̖��O���󂯎��A�t�@�C�����쐬����
//    OutputType�񋓑̂ŏo�͂���f�[�^���w�肵�A����ɉ��������O��_����
//----------------------------------------------------------------------------------------------
void FileHandler::CreateFile(std::string dirName, OutputType type)
{
	if (type == OutputType::position) fileName = CreatePosFile(dirName);	//�ʒu�f�[�^�t�@�C���쐬
	if (type == OutputType::velocity) fileName = CreateVelFile(dirName);	//���x�f�[�^�t�@�C���쐬
	if (type == OutputType::force)    fileName = CreateForceFile(dirName);	//�O�̓f�[�^�t�@�C���쐬
	file.open(fileName);
}

void FileHandler::CreateFile(std::string dirName, std::string filename)
{
	fileName = dirName + "/" + filename;
	file.open(fileName);
}

//----------------------------------------------------------------------------------------------
//    �t�@�C�����̃i���o�����O��ݒ肷��
//    ���̃t�@�C���̃i���o�����O���m�F���A������1�����傫�������l��ݒ肷��
//    std::stoi�ŕ������int�^�̒l�ɕϊ��ł��Ȃ��������A��O�𓊂���
//----------------------------------------------------------------------------------------------
void FileHandler::SetIndex(const std::string& dirName)
{
	int maxNum = 0;
	std::regex re(R"(\d{3})");	//"(����3��)�̐��K�\��

	try {
		//dirName���̃t�@�C����3���̐����Ŏn�܂���̂�T���A�ł��傫���������L�^����
		for (const auto& entry : std::filesystem::directory_iterator(dirName)) {
			std::string fileName = entry.path().filename().string();
			std::smatch match;

			//���K�\����"(����3��)�p����.csv�Ƀ}�b�`����t�@�C����T��
			if (std::regex_search(fileName, match, re)) {
				int num = std::stoi(match[0].str());

				//��ԑ傫���i���o�����O�̐�����ێ����Ă���
				if (num > maxNum) {
					maxNum = num;
				}
			}
		}
	}
	catch (const std::exception& e) {
		std::cerr << "An error occurred: " << e.what() << std::endl;
		std::abort();
	}
	
	index = maxNum + 1;	//��ԑ傫���i���o�����O��1�𑫂���index�ɓ���Ă���
	
}

//------------------------------------------------------------------------------------------------
//    csv�t�@�C�������p�A���ݎ������擾����
//------------------------------------------------------------------------------------------------
std::string FileHandler::GetCurrentTimeStr()
{
	auto now = std::chrono::system_clock::now();
	auto inTimeT = std::chrono::system_clock::to_time_t(now);

	std::tm buf;
	localtime_s(&buf, &inTimeT);
	std::stringstream ss;
	ss << std::put_time(&buf, "%Y%m%d");	//�N�����܂Ŏ擾����

	return ss.str();	//�擾�������Ԃ𕶎���ɕϊ����ĕԂ�
}

//------------------------------------------------------------------------------------------------
//    output�f�B���N�g����dirName�̃f�B���N�g�����Ȃ������ꍇ�A�f�B���N�g�����쐬����
//------------------------------------------------------------------------------------------------
void FileHandler::CreateDir(std::string dirName)
{
	if (!std::filesystem::exists(dirName)) {
		std::filesystem::create_directories(dirName);
	}
}

//----------------------------------------------------------------------------------------------
//    �t�@�C�����̃i���o�����O��Ԃ�
//----------------------------------------------------------------------------------------------
std::string FileHandler::GetIndex()
{
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << index;
	std::string indexStr = oss.str();
	return indexStr;
}

//----------------------------------------------------------------------------------------------
//    �t�@�C������Ԃ�
//----------------------------------------------------------------------------------------------
std::string FileHandler::GetName() const
{
	return fileName;
}


//----------------------------------------------------------------------------------------------
//    �t�@�C������Ԃ�
//----------------------------------------------------------------------------------------------
std::string FileHandler::FixedValueStr(int num, double var)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(num) << var;
	std::string result = oss.str();
	return result;
}

//----------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̈ʒu���o�͂���t�@�C�����쐬����
//----------------------------------------------------------------------------------------------
std::string FileHandler::CreatePosFile(std::string dirName)
{
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << index << "position.csv";

	std::string fileName = dirName + "/" + oss.str();
	return fileName;
}

//----------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̑��x���o�͂���t�@�C�����쐬����
//----------------------------------------------------------------------------------------------
std::string FileHandler::CreateVelFile(std::string dirName)
{
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << index << "velocity.csv";

	std::string fileName = dirName + "/" + oss.str();
	return fileName;
}

//----------------------------------------------------------------------------------------------
//    �{���e�b�N�X�̊O�͂��o�͂���t�@�C�����쐬����
//----------------------------------------------------------------------------------------------
std::string FileHandler::CreateForceFile(std::string dirName)
{
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << index << "force.csv";

	std::string fileName = dirName + "/" + oss.str();
	return fileName;
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�ݒ肵���p�����[�^���L�ڂ���
//----------------------------------------------------------------------------------------------
void FileHandler::WriteParam(Paramater<double> param) {
	file << "[Constant]\n";
	file << "EOM: " << param.EOM << "\n";
	file << "condition: " << param.condition << "\n";
	file << "vortexNum = " << param.vortexNum << "\n";
	file << "piningsiteNum = " << param.piningSiteNum << "\n";
	file << "dt = " << param.dt << "\n";
	file << "maxTime = " << param.maxTime << "\n";
	file << "a = " << param.a << "\n";
	file << "weight = " << param.weight << "\n";
	file << "height = " << param.height << "\n";
	file << "cutoff = " << param.cutoff << "\n";
	file << "eta = " << param.eta << "\n";
	file << "annealTime = " << param.annealTime << "\n";
	file << "lorentzFrequency = " << param.lorentzFrequency << "\n";
	file << "f0 = " << param.f0 << "\n";
	file << "kp = " << param.kp << "\n";
	file << "lp = " << param.lp << "\n";

	file << "\n[Variable]\n";
	file << param.var1name << " first: " << param.var1[0] << ", end: " 
		 << param.var1[1] << ", step: " << param.var1[2] << "\n";
	file << param.var2name << " first:  " << param.var2[0] << ", end: "
		 << param.var2[1] << ", step: " << param.var2[2] << "\n";

	
}


//----------------------------------------------------------------------------------------------
//     csv�t�@�C���������ݗp�A�s�j���O�T�C�g�̈ʒu�Ɣ��a���L�ڂ���
//----------------------------------------------------------------------------------------------
void FileHandler::WritePinPos(const unique_ptr<PiningSiteCircle[]>& piningsites, int pinNum)
{
	for (int i = 0; i < pinNum; i++) {
		file << "x" << i+1 << ",y" << i+1 << ",r" << i+1 << ",";
	}
	file << "\n";
	
	for (int i = 0; i < pinNum; i++) {
		file << piningsites[i].GetPos().x() << "," << piningsites[i].GetPos().y() << "," << piningsites[i].GetR() << ",";
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//     csv�t�@�C���������ݗp�A���x�����L�ڂ���
//----------------------------------------------------------------------------------------------
void FileHandler::WriteLabel(int vortexNum)
{
	file << "time";
	for (int i = 0; i < vortexNum; i++) {
		file << ",x"<<i+1<<",y"<<i+1;
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̈ʒu���t�@�C���ɏ�������(���Ԕ��W�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WritePos(double time, const unique_ptr<Vortex[]>& vortexs, int vortexNum)
{
	file << time;
	for (int i = 0; i < vortexNum; i++) {
		file << "," << vortexs[i].GetPos().x() << "," << vortexs[i].GetPos().y();
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̑��x���t�@�C���ɏ�������(���Ԕ��W�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteVelocity(double time, const unique_ptr<Vortex[]>& vortexs, int vortexNum)
{
	file << time;
	for (int i = 0; i < vortexNum; i++) {
		file << "," << vortexs[i].GetVelocity().x() << "," << vortexs[i].GetVelocity().y();
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̊O�͂��t�@�C���ɏ�������(���Ԕ��W�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteForce(double time, const unique_ptr<Vortex[]>& vortexs, int vortexNum)
{
	file << time;
	for (int i = 0; i < vortexNum; i++) {
		file << "," << vortexs[i].GetForce().x() << "," << vortexs[i].GetForce().y();
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̈ʒu���t�@�C���ɏ�������(���z�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WritePos(const unique_ptr<Vortex[]>& vortexs, int vortexNum)
{
	file << "x,y\n";
	for (int i = 0; i < vortexNum; i++) {
		file << vortexs[i].GetPos().x() << "," << vortexs[i].GetPos().y() << "\n";
	}
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̑��x���t�@�C���ɏ�������(���z�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteVelocity(const unique_ptr<Vortex[]>& vortexs, int vortexNum)
{
	file << "x,y\n";
	for (int i = 0; i < vortexNum; i++) {
		file << vortexs[i].GetVelocity().x() << "," << vortexs[i].GetVelocity().y() << "\n";
	}
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̊O�͂��t�@�C���ɏ�������(���z�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteForce(const unique_ptr<Vortex[]>& vortexs, int vortexNum)
{
	file << "x,y\n";
	for (int i = 0; i < vortexNum; i++) {
		file << vortexs[i].GetForce().x() << "," << vortexs[i].GetForce().y() << "\n";
	}
}