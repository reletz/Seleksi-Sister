{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
    distro-grub-themes.url = "github:AdisonCavani/distro-grub-themes";
  };
  outputs = { self, nixpkgs, ... }@inputs: {
    nixosConfigurations."default" = nixpkgs.lib.nixosSystem rec {
      system = "x86_64-linux";
      modules = [
        ./configuration.nix
        inputs.distro-grub-themes.nixosModules.${system}.default
      ];
    };
  };
}
