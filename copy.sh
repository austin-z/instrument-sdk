src_dir=/home/hlt/Develop/Code/instrument-sdk/build/sdk
dest_dir=/home/hlt/Develop/Code/instrument-example/sdk

rsync -a --force $src_dir/ $dest_dir

exit 0