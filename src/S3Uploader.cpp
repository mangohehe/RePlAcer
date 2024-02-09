// S3Uploader.cpp
#include "S3Uploader.h"
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <fstream>
#include <iostream>

S3Uploader::S3Uploader(const Aws::String& region) : region(region) {
    Aws::InitAPI(options);
}

S3Uploader::~S3Uploader() {
    Aws::ShutdownAPI(options);
}

bool S3Uploader::UploadFile(const std::string& file_name, const std::string& bucket_name, const std::string& object_name) {
    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = region;
    Aws::S3::S3Client s3_client(clientConfig);

    Aws::S3::Model::PutObjectRequest object_request;
    object_request.WithBucket(bucket_name.c_str()).WithKey(object_name.c_str());

    auto input_data = Aws::MakeShared<Aws::FStream>("PutObjectInputStream",
                                                    file_name.c_str(),
                                                    std::ios_base::in | std::ios_base::binary);

    object_request.SetBody(input_data);

    auto put_object_outcome = s3_client.PutObject(object_request);
    if (!put_object_outcome.IsSuccess()) {
        std::cerr << "Failed to upload file to S3: " << put_object_outcome.GetError().GetMessage() << std::endl;
        return false;
    }
    return true;
}

