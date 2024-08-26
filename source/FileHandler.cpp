#include "FileHandler.h"


//�R���X�g���N�^
FileHandler::FileHandler()
{
	fileName = "";
}

//�f�X�g���N�^
FileHandler::~FileHandler()
{
	/* DO_NOTHING */
}

//----------------------------------------------------------------------------------------------
//    �t�@�C�����̃i���o�����O��ݒ肷��
//    ���̃t�@�C���̃i���o�����O���m�F���A������1�����傫�������l��ݒ肷��
//----------------------------------------------------------------------------------------------
void FileHandler::SetIndex(std::string dirName)
{
	int maxNum = 0;
	std::regex re(R"(\d{3}[a-zA-Z]+\.[a-zA-Z]+)");	//"(����3��)�p����.csv�Ƀ}�b�`���鐳�K�\��

	for (const auto& entry : std::filesystem::directory_iterator(dirName)) {
		std::string fileName = entry.path().filename().string();
		std::smatch match;

		//���K�\����"(����3��)�p����.csv�Ƀ}�b�`����t�@�C����T��
		if (std::regex_match(fileName, match, re)) {
			int num = std::stoi(match[1].str());

			//��ԑ傫���i���o�����O�̐�����ێ����Ă���
			if (num > maxNum) {
				maxNum = num;
			}
		}
	}
	index = maxNum + 1;	//��ԑ傫���i���o�����O��1�𑫂���index�ɓ���Ă���
}

//----------------------------------------------------------------------------------------------
//    �t�@�C������ݒ肷��
//----------------------------------------------------------------------------------------------
void FileHandler::SetName(std::string name)
{
	fileName = name;
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
std::string FileHandler::CreateFilePos(std::string dirName)
{
	std::ostringstream oss;
	oss << std::setw(3) << std::setfill('0') << index << "position.csv";

	std::string fileName = dirName + "/" + oss.str();
	return fileName;
}

//----------------------------------------------------------------------------------------------
//     csv�t�@�C���������ݗp�A���x�����L�ڂ���
//----------------------------------------------------------------------------------------------
void FileHandler::WriteLabel(std::ofstream& file, int voltexNum)
{
	file << "time";
	for (int i = 0; i < voltexNum; i++) {
		file << ",x,y";
	}
	file << "\n";
}

//----------------------------------------------------------------------------------------------
//    
//----------------------------------------------------------------------------------------------
void FileHandler::WriteAll(std::ofstream& file, double time)
{

}

//----------------------------------------------------------------------------------------------
//    csv�t�@�C���������ݗp�A�e�{���e�b�N�X�̈ʒu���t�@�C���ɏ�������
//----------------------------------------------------------------------------------------------
void FileHandler::WritePos(std::ofstream& file, double time, const unique_ptr<Voltex[]>& voltexs, int voltexNum)
{
	file << time;
	for (int i = 0; i < voltexNum; i++) {
		file << "," << voltexs[i].GetPos().x() << "," << voltexs[i].GetPos().y();
	}
	file << "\n";
}