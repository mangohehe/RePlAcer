#ifndef S3DOWNLOADER_H
#define S3DOWNLOADER_H

#include <string>
#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>

class S3Downloader {
public:
    S3Downloader();
    ~S3Downloader();

    void Init(const Aws::SDKOptions& options);
    void Shutdown();
    bool DownloadFile(const std::string& bucketName, const std::string& objectName, const std::string& outputPath);

private:
    Aws::S3::S3Client m_s3_client;
    bool m_isInitialized;
};

#endif // S3DOWNLOADER_H
