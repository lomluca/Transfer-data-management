// File DLL principale.

#include "stdafx.h"

#include "TransferDataManagement.h"
#include <msclr\marshal_cppstd.h>

TransferDataManagement::ServerWrapper::ServerWrapper(String^ address, String^ port, String^ path_root, int threadPoolSize, String^ clientDownloadPath, bool auto_download)
{
	std::string u_address = msclr::interop::marshal_as<std::string>(address);
	std::string u_port = msclr::interop::marshal_as<std::string>(port);
	std::string u_path_root = msclr::interop::marshal_as<std::string>(path_root);
	std::string u_clientDownloadPath = msclr::interop::marshal_as<std::string>(clientDownloadPath);
	boost::replace_all(u_clientDownloadPath, "C:", "");
	u_clientDownloadPath += "\\";
	std::cout << u_path_root << " " << u_clientDownloadPath << std::endl;
	instance = new http::server3::server(u_address, u_port, u_path_root, threadPoolSize, u_clientDownloadPath, auto_download);
}

void TransferDataManagement::ServerWrapper::runWrapper()
{
	instance->run();
}

void TransferDataManagement::ServerWrapper::setDownloadPath(String^ path) {
	std::string u_clientDownloadPath = msclr::interop::marshal_as<std::string>(path);
	boost::replace_all(u_clientDownloadPath, "C:", "");
	u_clientDownloadPath += "\\";
	instance->setClientDownloadPath(u_clientDownloadPath);
}

void TransferDataManagement::ServerWrapper::setAutoDownload(bool newValue) {
	instance->setAutomaticAccept(newValue);
}

void TransferDataManagement::ServerWrapper::stopServer() {
	instance->handle_stop();
}

TransferDataManagement::Connection^ TransferDataManagement::ServerWrapper::getNext()
{
	http::server3::connection_ptr conn = instance->getConnection();
	Connection^ c = gcnew Connection(conn.get());
	return c;
}

TransferDataManagement::Connection::Connection(http::server3::connection * existing)
{
	instance = existing;
}

void TransferDataManagement::Connection::changeDownloadPath(String^ newPath) {
	std::string u_new_path = msclr::interop::marshal_as<std::string>(newPath);
	boost::replace_all(u_new_path, "C:", "");
	u_new_path += "\\";
	instance->setClientDownloadPath(u_new_path);
}

void TransferDataManagement::Connection::answer(bool value) {
	instance->setAcceptFile(value);
}

void TransferDataManagement::Connection::setStopFlag(bool flag) {
	instance->setStopFlag(flag);
}

void TransferDataManagement::Connection::stop() {
	DownloadInfo* d_info = instance->getDownloadInfo();
	d_info->setOnExit(true);
	instance->setOnExit(true);
}

String^ TransferDataManagement::Connection::getStatus()
{
	 DownloadInfo* d_info = instance->getDownloadInfo();
	 std::string status = d_info->getStatus();
	 String^ m_status = gcnew String(status.c_str());
	 return m_status;
}

bool TransferDataManagement::Connection::isValid() {
	try {
		DownloadInfo* d_info = instance->getDownloadInfo();
		return true;
	} catch (std::exception& e) {
		return false;
	}
}

bool TransferDataManagement::Connection::mustBeShown() {
	if (instance->getRequestMethod() == "GET") {
		return true;
	}
	return false;
}

double TransferDataManagement::Connection::getByteSent()
{
	DownloadInfo* d_info = instance->getDownloadInfo();
	double byteSent = d_info->getBytesSent();
	return byteSent;
}

double TransferDataManagement::Connection::getSize()
{
	DownloadInfo* d_info = instance->getDownloadInfo();
	double size = d_info->getTotSize();
	return size;
}

double TransferDataManagement::Connection::getRemainingTime()
{
	DownloadInfo* d_info = instance->getDownloadInfo();
	double time = d_info->getTime();
	return time;
}

TransferDataManagement::ClientWrapper::ClientWrapper(String ^ server, String ^ path, String ^ downloadPath)
{
	io = new boost::asio::io_service();
	std::string u_server = msclr::interop::marshal_as<std::string>(server);
	std::string u_path = msclr::interop::marshal_as<std::string>(path);
	std::string u_downloadPath = msclr::interop::marshal_as<std::string>(downloadPath);
	boost::replace_all(u_path, "C:\\", "");
	std::cout << "Il file da inviare è: " << u_path << std::endl;
	instance = new httpClient(u_server, u_path, *io, u_downloadPath);
}

void TransferDataManagement::ClientWrapper::start()
{
	instance->sendTAKE();
}

String ^ TransferDataManagement::ClientWrapper::getStatus()
{
	DownloadInfo d_info = instance->getDownloadInfo();
	std::string status = d_info.getStatus();
	String^ m_status = gcnew String(status.c_str());
	return m_status;
}

double TransferDataManagement::ClientWrapper::getByteSent()
{
	DownloadInfo d_info = instance->getDownloadInfo();
	double byteSent = d_info.getBytesSent();
	return byteSent;
}

double TransferDataManagement::ClientWrapper::getSize()
{
	DownloadInfo d_info = instance->getDownloadInfo();
	double size = d_info.getTotSize();
	return size;
}

double TransferDataManagement::ClientWrapper::getRemainingTime()
{
	DownloadInfo d_info = instance->getDownloadInfo();
	double time = d_info.getTime();
	return time;
}
