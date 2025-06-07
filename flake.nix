{
    description = "x86_64 dev environment for Shwet, the whiteboard app.";
    inputs = {
        nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.11";
    };
    outputs = { self, nixpkgs }:
    let
        system = "x86_64-linux";
        pkgs = nixpkgs.legacyPackages.${system};
    in
    {
        devShells.${system}.default = pkgs.mkShell {
            buildInputs = with pkgs; [
                qt6.full
                qtcreator
                cmake
                bear
            ];
          shellHook = ''
            which zsh >/dev/null && exec zsh
          '';
        };
    };
}
