{ lib
, stdenv
, cmake

, gtest
, antlr
, commonroad-cmake

, enableSanitizers ? false
}:

stdenv.mkDerivation rec {
  pname = "rv-monitors";
  version = "0.5.0";

  src = ./.;

  nativeBuildInputs = [
    cmake
    antlr.jre
  ];

  buildInputs = [
    antlr
  ];

  checkInputs = [
    gtest
  ];

  propagatedBuildInputs = [
    antlr.runtime.cpp
  ];

  #cmakeBuildType = "RelWithDebInfo";
  #separateDebugInfo = true;

  doCheck = true;

  preCheck = lib.optional enableSanitizers "export ASAN_OPTIONS=detect_leaks=0";

  cmakeFlags = [
    "-DFETCHCONTENT_QUIET=OFF"
    "-DFETCHCONTENT_SOURCE_DIR_COMMONROAD_CMAKE=${commonroad-cmake}"
    "-DANTLR4_JAR_LOCATION=${antlr.jarLocation}"
  ] ++ lib.optional enableSanitizers "-DCOMMONROAD_SANITIZERS=ASAN_UBSAN";

  meta = {
    homepage = "https://gitlab.lrz.de/commonroad-traffic-rules/rv-monitors";
    description = ''
      A C++17 Implementation of four runtime verification monitor synthesis algorithms [1]
      for LTLf and PLTLf formulas on finite traces
    '';
    # license = lib.licenses.mit;
    platforms = with lib.platforms; linux ++ darwin;
  };
}
