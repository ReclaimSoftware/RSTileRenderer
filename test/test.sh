cd .. &&
    bash build.sh &&
    cat test/tiledata/15-kc-mo9-merge | \
        ./build/release/tile-renderer-mac --zoom 15 --format png > \
        test/rendered/15-kc-mo9-merge.png && \
    cat test/tiledata/18-ny-fidi | \
        ./build/release/tile-renderer-mac --zoom 18 --format png > \
        test/rendered/18-ny-fidi.png &&
    cd test/rendered &&
    optipng -silent -o3 *
