#include "S3Downloader.h"
#include <aws/s3/model/GetObjectRequest.h>
#include <fstream>
#include <iostream>

S3Downloader::S3Downloader() : m_isInitialized(false) {}

S3Downloader::~S3Downloader() {
    if (m_isInitialized) {
        Shutdown();
    }
}

void S3Downloader::Init(const Aws::SDKOptions& options) {
    if (!m_isInitialized) {
        Aws::InitAPI(options);
        Aws::Client::ClientConfiguration clientConfig;
        m_s3_client = Aws::S3::S3Client(clientConfig);
        clientConfig.region = Aws::Region::US_EAST_1;
        m_isInitialized = true;
    }
}

void S3Downloader::Shutdown() {
    if (m_isInitialized) {
        Aws::ShutdownAPI(Aws::SDKOptions());
        m_isInitialized = false;
    }
}

bool S3Downloader::DownloadFile(const std::string& bucketName, const std::string& objectName, const std::string& outputPath) {
    if (!m_isInitialized) {
        std::cerr << "AWS SDK is not initialized. Call Init first." << std::endl;
        return false;
    }

    Aws::S3::Model::GetObjectRequest object_request;
    object_request.WithBucket(bucketName).WithKey(objectName);

    auto get_object_outcome = m_s3_client.GetObject(object_request);

    if (get_object_outcome.IsSuccess()) {
        Aws::OFStream local_file;
        local_file.open(outputPath, std::ios::out | std::ios::binary);
        local_file << get_object_outcome.GetResult().GetBody().rdbuf();
        return true;
    } else {
        std::cerr << "Failed to download file from S3: " << get_object_outcome.GetError().GetMessage() << std::endl;
        return false;
    }
}
