#Usage
#   ./make_folder.sh dir
#Creates directories named 'a' to 'z' in `dir`

for i in {97..122}; do
    a=$(printf "\x$(printf %x $((i)))")
    mkdir $1/$a
done
