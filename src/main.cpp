#include "SpinParser.hpp"
#ifndef DISABLE_MPI
#include "mpi.h"
#endif

int main(int argc, char **argv)
{
	//init MPI ����Ѿ�"#define DISABLE_MPI"��������
	#ifndef DISABLE_MPI
	int status = MPI_Init(&argc, &argv);
	if (status != MPI_SUCCESS) throw Exception(Exception::Type::MpiError, status);//���MPI��ʼ��ʧ���׳��쳣
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank != 0) Log::log << Log::setDisplayLogLevel(Log::LogLevel::None);
	#endif

	//���� SpinParser ����
	SpinParser *spinParser = SpinParser::spinParser();
	int returnCode = spinParser->run(argc, argv);

	//finalize MPI
	#ifndef DISABLE_MPI
	MPI_Finalize();
	#endif

	//return
	return returnCode;
}