// S3Uploader.h
#ifndef S3_UPLOADER_H
#define S3_UPLOADER_H

#include <string>
#include <aws/core/Aws.h>

class S3Uploader {
public:
    S3Uploader();
    ~S3Uploader();
    bool UploadData(const std::string& data, const std::string& bucket_name, const std::string& object_name);

private:
    Aws::SDKOptions options;
};

#endif // S3_UPLOADER_H

