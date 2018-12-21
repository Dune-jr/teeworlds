with import <nixpkgs> {}; {
    teeworldsEnv = stdenv.mkDerivation {
        name = "teeworlds-env";
        buildInputs = [ stdenv
                        gcc
                        SDL2
                        freetype
                        libGLU
                      ];
    };
}
