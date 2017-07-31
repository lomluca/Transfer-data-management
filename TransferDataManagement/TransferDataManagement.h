// TransferDataManagement.h

#pragma once

using namespace System;

#include "..\..\ConsoleApplication3\ConsoleApplication3\server.h"

namespace TransferDataManagement {

	public ref class Connection
	{
	public:
		Connection(http::server3::connection* existing);
		bool isValid();
		String^ getStatus();
		double getByteSent();
		double getSize();
		double getRemainingTime();
		void setStopFlag(bool flag);
		bool mustBeShown();
		void answer(bool ans);
		void changeDownloadPath(String^ newPath);
		void stop();

	private:
		http::server3::connection* instance;
	};

	public ref class ClientWrapper
	{
	public:
		ClientWrapper(String^ server, String^ path, String^ downloadPath);
		void start();
		String^ getStatus();
		double getByteSent();
		double getSize();
		double getRemainingTime();

	private:
		httpClient* instance;
		boost::asio::io_service* io;
	};

	public ref class ServerWrapper
	{
	public:
		ServerWrapper(String^ address, String^ port, String^ path_root, int threadPoolSize, String^ clientDownloadPath, bool autoDownload);
		void runWrapper();
		Connection^ getNext();
		void stopServer();
		void setAutoDownload(bool newValue);
		void setDownloadPath(String^ path);

	private:
		http::server3::server *instance;
	};
}
