for dir in ./*/
do
  dir=${dir%*/}

  if test -f "${dir##*/}/project.yml";
  then
    echo "Ceedling project found @ ${dir%*/}. Testing all... "
    cd "${dir##*/}/"
    (ceedling test:all && echo "Test @ ${dir%*/} Ok!") || echo "Test @ ${dir%*/} failed!"
    cd ..
  fi
done