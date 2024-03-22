task_id="5db398fa-b797-4ac2-bf7c-a00d1a2a2cd8"
docker run \
-e CONFIG_S3_KEY="public/configs/${task_id}-placement-config.json" \
-e CONFIG_S3_BUCKET="semidesignhub-storage-design-settings224617-staging" \
-e OUTPUT_S3_BUCKET="semidesignhub-storage-design-settings224617-staging" \
-e OUTPUT_S3_KEY="public/output/${task_id}-placement-output.def" \
-e AWS_ACCESS_KEY_ID="AKIAVK4EQS5WCAKSXU4T" \
-e AWS_SECRET_ACCESS_KEY="NQVV45cbAhetEApTfjj70HjgC59m+O3sCk43vBC4" \
replace-ubuntu20.04

