// S3Uploader.h
#ifndef S3_UPLOADER_H
#define S3_UPLOADER_H

#include <string>
#include <aws/core/Aws.h>

class S3Uploader {
public:
    S3Uploader(const Aws::String& region);
    ~S3Uploader();
    bool UploadFile(const std::string& file_name, const std::string& bucket_name, const std::string& object_name);

private:
    Aws::SDKOptions options;
    Aws::String region;
};

#endif // S3_UPLOADER_H

