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
//    output�f�B���N�g���ɍ����̓��t�̃f�B���N�g�����Ȃ������ꍇ�A�f�B���N�g�����쐬����
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
int FileHandler::GetIndex() const
{
	return index;
}

//----------------------------------------------------------------------------------------------
//    �t�@�C������Ԃ�
//----------------------------------------------------------------------------------------------
std::string FileHandler::GetName() const
{
	return fileName;
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
//     csv�t�@�C���������ݗp�A���x�����L�ڂ���
//----------------------------------------------------------------------------------------------
void FileHandler::WriteLabel(int voltexNum)
{
	file << "time";
	for (int i = 0; i < voltexNum; i++) {
		file << ",x,y";
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̈ʒu���t�@�C���ɏ�������(���Ԕ��W�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WritePos(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << time;
	for (int i = 0; i < voltexNum; i++) {
		file << "," << voltexs[i].GetPos().x() << "," << voltexs[i].GetPos().y();
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̑��x���t�@�C���ɏ�������(���Ԕ��W�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteVelocity(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << time;
	for (int i = 0; i < voltexNum; i++) {
		file << "," << voltexs[i].GetVelocity().x() << "," << voltexs[i].GetVelocity().y();
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̊O�͂��t�@�C���ɏ�������(���Ԕ��W�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteForce(double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << time;
	for (int i = 0; i < voltexNum; i++) {
		file << "," << voltexs[i].GetForce().x() << "," << voltexs[i].GetForce().y();
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̈ʒu���t�@�C���ɏ�������(���z�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WritePos(const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << "x,y\n";
	for (int i = 0; i < voltexNum; i++) {
		file << voltexs[i].GetPos().x() << "," << voltexs[i].GetPos().y() << "\n";
	}
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̑��x���t�@�C���ɏ�������(���z�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteVelocity(const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << "x,y\n";
	for (int i = 0; i < voltexNum; i++) {
		file << voltexs[i].GetVelocity().x() << "," << voltexs[i].GetVelocity().y() << "\n";
	}
}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̊O�͂��t�@�C���ɏ�������(���z�p)
//----------------------------------------------------------------------------------------------
void FileHandler::WriteForce(const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << "x,y\n";
	for (int i = 0; i < voltexNum; i++) {
		file << voltexs[i].GetForce().x() << "," << voltexs[i].GetForce().y() << "\n";
	}
}