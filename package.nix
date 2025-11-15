{
  stdenv,
  lib,
  fetchFromGitHub,
  makeDesktopItem,
  cmake,
  wrapQtAppsHook,
  qtEnv,
  qt6,
}:

let
  icon = makeDesktopItem {
    name = "Drawy";
    exec = "drawy";
    icon = "drawy";
    comment = "Your handy, infinite, brainstorming tool.";
    desktopName = "Drawy";
    genericName = "Drawy";
    categories = [ "Graphics" ];
  };
in
stdenv.mkDerivation rec {
  pname = "drawy";
  version = "1.0.0";

  src = fetchFromGitHub {
    owner = "Prayag2";
    repo = pname;
    rev = "1.0.0-alpha";
    sha256 = "sha256-5hv6iBTXTXwsmtjzRA+dAIzx/5jtWcOEfORMK0l1DSk=";
  };

  nativeBuildInputs = [
    cmake
    wrapQtAppsHook
  ];

  buildInputs = [
    qtEnv
    qt6.qtbase
  ];

  meta = with lib; {
    description = "Your handy, infinite, brainstorming tool";
    homepage = "https://github.com/Prayag2/drawy";
    license = licenses.gpl3;
    platforms = platforms.linux;
  };
}
