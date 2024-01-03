{
  inputs = {
    nixpkgs.url = "nixpkgs/nixos-unstable";

    utils.url = "github:numtide/flake-utils";

    commonroad-cmake = {
      url = "git+https://gitlab.lrz.de/tum-cps/commonroad-cmake.git";
      flake = false;
    };
  };

  outputs = { self, nixpkgs, ... }@inputs:
    let
      supportedSystems = [
        "x86_64-linux"
        "i686-linux"
        "aarch64-linux"
        "x86_64-darwin"
      ];
    in
    inputs.utils.lib.eachSystem supportedSystems (system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
      in
      rec {
        packages = rec {
          rv-monitors = pkgs.callPackage ./default.nix {
            inherit (inputs) commonroad-cmake;
            antlr = pkgs.antlr4.override {
              jre = pkgs.jre_minimal;
            };
          };
          default = rv-monitors;
        };

        formatter = pkgs.nixpkgs-fmt;
      });
}
