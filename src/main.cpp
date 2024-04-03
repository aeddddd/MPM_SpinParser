#include "SpinParser.hpp"
#ifndef DISABLE_MPI
#include "mpi.h"
#endif

int main(int argc, char **argv)
{
	//init MPI 如果已经"#define DISABLE_MPI"则加载语句
	#ifndef DISABLE_MPI
	int status = MPI_Init(&argc, &argv);
	if (status != MPI_SUCCESS) throw Exception(Exception::Type::MpiError, status);//如果MPI初始化失败抛出异常
	int rank;
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank != 0) Log::log << Log::setDisplayLogLevel(Log::LogLevel::None);
	#endif

	//启动 SpinParser 计算
	SpinParser *spinParser = SpinParser::spinParser();
	int returnCode = spinParser->run(argc, argv);

	//finalize MPI
	#ifndef DISABLE_MPI
	MPI_Finalize();
	#endif

	//return
	return returnCode;
}