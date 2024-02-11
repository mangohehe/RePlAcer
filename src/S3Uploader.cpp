#include "S3Uploader.h"
#include <aws/s3/S3Client.h>
#include <aws/s3/model/PutObjectRequest.h>
#include <fstream>
#include <iostream>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/core/utils/logging/LogLevel.h>
#include <cstdio> // For printf

S3Uploader::S3Uploader() {
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Debug;
    Aws::InitAPI(options);
}

S3Uploader::~S3Uploader() {
    Aws::ShutdownAPI(options);
}

bool S3Uploader::UploadData(const std::string& data, const std::string& bucket_name, const std::string& object_name) {
    Aws::Client::ClientConfiguration clientConfig;
    clientConfig.region = Aws::Region::US_EAST_1;
    Aws::S3::S3Client s3_client(clientConfig);
    Aws::S3::Model::PutObjectRequest object_request;

    object_request.WithBucket(bucket_name.c_str()).WithKey(object_name.c_str());
    auto stream = Aws::MakeShared<Aws::StringStream>("UploadDataStream");
    *stream << data; // Fill the stream with the data to upload
    object_request.SetBody(stream);

    try {
    	printf("Uploading file: feng test 4");

        auto put_object_outcome = s3_client.PutObject(object_request);
        if (!put_object_outcome.IsSuccess()) {
        	printf("Uploading file: feng test 4");
        	printf("Feng: Failed to upload file to S3: %s (HTTP Response Code: %d) (AWS Request ID: %s)\n",
        	       put_object_outcome.GetError().GetMessage().c_str(),
        	       static_cast<int>(put_object_outcome.GetError().GetResponseCode()), // Cast to int
        	       put_object_outcome.GetError().GetRequestId().c_str());

            return false;
        } else {
            printf("Successfully uploaded file to S3. Bucket: %s, Key: %s\n", bucket_name.c_str(), object_name.c_str());
        }
    } catch (const Aws::Client::AWSError<Aws::S3::S3Errors>& e) {
        printf("S3 Upload Error: %s - %s\n", e.GetExceptionName().c_str(), e.GetMessage().c_str());
        return false;
    } catch (const std::exception& e) {
        printf("Standard Exception: %s\n", e.what());
        return false;
    } catch (...) {
        printf("An unknown error has occurred\n");
        return false;
    }
	printf("Uploading file: feng test 5");

    return true;
}
