[![icon](/data/pixmaps/idbutton.png?raw=true)](https://www.darktable.org/) R&Darktable
============

## Foreword

### Dr Rant got fed up

**Rational & Darktable** is a fork of the official darktable software with
GUI changes that essentially aim at removing clutter and options.

I strongly disagree with
the latest decisions of the dev team that has proven to favour
over-engineered solutions to simple problems, leading to brittle code, regressions and less efficient usage.

The trend in darktable development lately has been to add many
options to avoid making design decisions instead of focusing on an
unified, consistent and streamlined workflow, in an attempt to pretend that everyone can be right at the same time. This is slowly heading
toward a mix between Microsoft and Dassault Systems software design.

ART (Another RawTherapee) has successfully shown that stripping away an
old project from its legacy of bloat indeed verifies the saying : less is more.

### Interoperability with upstream darktable

This fork is and will aim at staying compatible with upstream darktable
regarding the pixel processing pipeline, modules, old histories and so on.
Edits created with this fork or upstream darktable can be open in either
of the apps.

GUI changes introduced in upstream darktable may not and will probably not
be backported here.

### Will you keep working on upstream darktable ?

**I will keep posting and maintaining my image-processing modules** on both repositories, fix bugs, optimize pixel code, write documentation, blog posts, assist users, and make videos about darktable.

I will not engage in large-scale refactorings or GUI changes as I'm tired of arguing with
people who should probably edit more pictures to feel the pain.

In any case, my opinion for a long time has been that darktable is approaching End Of Life.
Its pixel pipeline has not been designed to be extended that far, and nobody understands
its internal circuitery anymore. Its performance can't be improved either, since images
start their life on CPU, are copied on GPU to be processed by OpenCL kernels (controlled anyway from the CPU - aka host), then brought back to the CPU to be painted in the GUI by Qtk
(single-threaded), then sent again to the GPU to be displayed on screen. No matter how much you optimize the pixel code, as long as
you don't remove those back-and-forth copies between RAM and vRAM,
the performance penalty will stay substantial.

Not to mention, OpenCL can't process real-time user input, like drawing or extracting image content, because it is meant mostly for data-science and low-level computation.

These shortcomings can be solved only by a full rewrite of the pipeline in Vulkan, but darktable's code intimately interleave pixel code with Gtk UI code.

For this reason, @hanatos, the founder of darktable, has started over a complete rewrite of
an image processing app for Vulkan: [vkdt](https://github.com/hanatos/vkdt). It is already
used by non-programmers to edit real pictures, but needs love to be comfortably usable.

I plan on joining effort with him in gradually during 2022, to port the scene-referred modules
of darktable to the performance of Vulkan and bring my knowledge of
picture editing to reboot a consistent workflow app from scratch.

During this work, legacy darktable/R&Darktable should get you covered to do your daily edits. That's what I use anyway and darktable 4.0
will be pretty much feature-complete as far as I'm concerned… The only
things I miss are already available in vkdt and not
realistic to implement in legacy dt for performance reasons.


## Back to your usual program

darktable is an open source photography workflow application and non-destructive raw developer - a virtual lighttable and darkroom for photographers. It manages your digital negatives in a database, lets you view them through a zoomable lighttable and enables you to develop raw images, enhance them and export them to local or remote storage.

![screenshot_lighttable](https://user-images.githubusercontent.com/45535283/148689197-e53dd75f-32f1-4297-9a0f-a9547fd4e7c7.jpg)

darktable is **not** a free Adobe® Lightroom® replacement.

[https://www.darktable.org/](https://www.darktable.org/ "darktable homepage")

## Table of Contents

1. [Documentation](#documentation)
2. [Website](#website)
3. [Requirements](#requirements)
   - [Supported platforms](#supported-platforms)
   - [Hardware](#hardware)
4. [Installing](#installing)
   - [Latest release](#latest-release)
   - [Development snapshot](#development-snapshot)
5. [Updating from older versions](#updating-from-older-versions)
6. [Obtaining extensions](#obtainin-extensions)
7. [Building](#building)
   - [Dependencies](#dependencies)
   - [Get the source](#get-the-source)
   - [Get submodules](#get-submodules)
   - [Compile](#compile)
   - [Further reading](#further-reading)
8. [Using](#using)
   - [Test/unstable version](#testunstable-version)
   - [Regular/stable version](#regularstable-version)
9. [Contributing](#contributing)
10. [FAQ](#faq)
   - [Why is my camera not detected when plugged-in ?](#why-is-my-camera-not-detected-when-plugged-in-)
   - [Why is my lens not detected/corrected in darkroom ?](#why-is-my-lens-not-detectedcorrected-in-darkroom-)
   - [Why do the thumbnails in the lighttable view look different to the preview in the darkroom view ?](#why-do-the-thumbnails-in-the-lighttable-view-look-different-to-the-preview-in-the-darkroom-view-)
11. [Wiki](#wiki)
12. [Mailing lists](#mailing-lists)

Documentation
-------------

The darktable user manual is maintained in the [dtdocs](https://github.com/darktable-org/dtdocs) repository.

Lua API documentation is maintained in the [luadocs](https://github.com/darktable-org/luadocs) repository.

Website
-------

The website ([https://www.darktable.org/](https://www.darktable.org/)) is maintained in the [dtorg](https://github.com/darktable-org/dtorg) repository.

Requirements
------------

### Supported platforms

* Linux (64 bit)
* Free BSD (64 bit)
* Windows 8 (64 bit), Windows 10 (64 bit)
* macOS

*32 bit platforms are not officially supported - they might or might not work.*

*Windows support is still young and suffers from bugs that do not affect Linux. If possible,
prefer using darktable on Linux.*

### Hardware

(workable minimum / **recommended minimum**):
* RAM: 4 GB / **8 GB**
* CPU: Intel Pentium 4 / **Intel Core i5 4×2.4 GHz**
* GPU: none / **Nvidia with 1024 CUDA cores, 4 GB, OpenCL 1.2 compatible**
* free disk space: 250 MB / **1 GB**

*darktable can run on lightweight configurations (even on a Raspberry Pi), but expect modules like denoise, local contrast,
contrast equalizer, retouch or liquify to be slow beyond usable.*

*A GPU is not mandatory but is strongly recommended for a smoother experience.
Nvidia GPUs are recommended for safety because some AMD drivers behave unreliably with some modules (e.g. local contrast).*

Installing
----------

If the latest release is still not available as a pre-built package for your distribution,
you can build the software yourself following the instructions [below](#building).

### Latest release

3.8.1 (stable)

* [Download executable for Windows](https://github.com/darktable-org/darktable/releases/download/release-3.8.1/darktable-3.8.1-win64.exe)
* [Download executable for mac OS](https://github.com/darktable-org/darktable/releases/download/release-3.8.1/darktable-3.8.1.dmg)
* [Install native packages and repositories for Linux](https://software.opensuse.org/download.html?project=graphics:darktable:stable&package=darktable)
* [Install Flatpak package for Linux](https://flathub.org/apps/details/org.darktable.Darktable)
* [More information about installing darktable on any system](https://www.darktable.org/install/)

*When using a pre-built package, ensure that it has been built with Lua, OpenCL, OpenMP and Colord support.
These are optional and will not prevent darktable from running if missing,
but their absence will degrade the user experience.
Notably, some Flatpak, Snap and Appimage packages lack OpenCL and Lua support.*

### Development snapshot

The development snapshot reflects the current state of the master branch. It is intended for testing and is generally not safe. See the notes [below](#get-the-source) for warnings and precautions about using the master branch.

* [Install native packages and repositories for Linux](https://software.opensuse.org/download.html?project=graphics:darktable:master&package=darktable) (one snapshot per day).
* No pre-compiled packages are provided for the master branch on macOS and Windows. See how to build it manually below.

Updating from older versions
----------------------------

When updating darktable from an older release, you only need install
the newest version. Existing files will be preserved.

However, newer releases occasionally need to change the structure of the library database
(containing the whole list of images known to darktable, with their editing history). If this happens
you will be prompted with a request to either upgrade the database or close the software.

**Migration to a newer database structure/newer release means that your edits (both new and old)
will no longer be compatible with older versions of darktable.** Upgrades are definitive.
Newer versions are always compatible with older edits, but newer edits are generally
not compatible with older versions.

darktable automatically backs up the library database when a new version causes it to be upgraded
(in `~/.config/darktable/library.db-pre-3.0.0` for example), so
you can revert to the previous release by restoring this backup if needed
(simply rename it to `library.db`).

If you try to open a newer database with an older version of the software, any portions of your edits that were
undertaken with new features will be discarded and you will lose them. This also applies to the sidecar XMP files.

If you plan to move regularly between two versions (new/unstable and old/stable) see [below](#testunstable-version)
for details of how to do it safely.

Obtaining extensions
--------------------

Extensions and plugins use the Lua scripting language and can be downloaded [here](https://github.com/darktable-org/lua-scripts). Lua support is optional in darktable, so make sure you have the `lua` interpreter and its development files (package
`lua-dev` or `lua-devel`, depending on distributions) installed on your system
while building or ensure the package you are using has been built with this library.

Extensions allow exporting for various media and websites, merge/stack/blend HDR, panoramas or focus bracketing,
apply AI-based facial recognition, manage tags and GPS data, etc.

Building
--------

### Dependencies

Compatible compilers:
* Clang: 8, 9, 10
* GCC: 8, 9, 10
* Mingw64: 6, 7

Required dependencies (minimum version):
* CMake 3.10
* Gtk 3.22
* Glib 2.40
* SQLite 3.15 (but 3.24 strongly recommended)

Optional dependencies (minimum version):
* OpenMP 4.5 *(for CPU multi-threading and SIMD vectorization)*
* LLVM 3.9 *(for OpenCL checks at compilation time)*
* OpenCL 1.2 *(for GPU-accelerated computing)*
* Lua 5.4 *(for plugins and extension scripting)*
* libavif 0.7.2 *(for AVIF import/export)*
* WebP 0.3.0 *(for WebP exports)*

Optional dependencies (no version requirement):
* Gphoto2 *(for camera tethering)*
* Lensfun *(for automatic lens correction)*
* OpenEXR *(for EXR import and export)*
* OpenJPEG *(for Jpeg2000 export)*
* Colord, Xatom *(for fetching the system display color profile)*
* G'Mic *(for gmz compressed lut support)*
* PortMidi *(for MIDI input support)*
* SDL2 *(for gamepad input support)*

To install all the dependencies on Linux systems, you may use the source repositories of your distribution
(provided they are up-to-date):

#### Fedora and RHEL

```bash
sudo dnf builddep darktable
```

#### OpenSuse

```bash
sudo zypper si -d darktable
```

#### Ubuntu

```bash
sed -e '/^#\sdeb-src /s/^# *//;t;d' "/etc/apt/sources.list" \
  | sudo tee /etc/apt/sources.list.d/darktable-sources-tmp.list > /dev/null \
  && (
    sudo apt-get update
    sudo apt-get build-dep darktable
  )
sudo rm /etc/apt/sources.list.d/darktable-sources-tmp.list
```

#### Debian

```bash
sudo apt-get build-dep darktable
```

#### Install missing dependencies

If mandatory dependencies are missing on your system, the software build will fail with
errors like `Package XXX has not been found` or `Command YYY has no provider on this system`.
If you see one of these errors you should find out which package provides the missing package/command in your distribution,
then install it. This can usually be done in your package manager (not the application manager
customarily provided by default in your distribution) or from the internet with a search engine.
You may need to install a package manager first (like Synaptic on Debian/Ubuntu, or DNF Dragora on Fedora/RHEL).

This process might be tedious but you only need to do it once. See
[this page on building darktable](https://github.com/darktable-org/darktable/wiki/Building-darktable)
for one-line commands that will install most dependencies on the most common Linux distributions.

### Get the source

#### Master branch (unstable)

The master branch contains the latest version of the source code and is intended:
* as a working base for developers,
* for beta-testers to chase bugs,
* for users willing to sacrifice stability for new features without waiting for the next release.

The master branch comes with no guarantee of stability and might corrupt your database and XMP files,
result in loss of data and edit history or temporarily break compatibility with previous versions and commits.

How dangerous is it? Most of the time, it is fairly stable. As with any rolling-release kind of deployment, bugs appear more often
but are fixed faster too. Sometimes, though, these bugs can result in losses or inconsistencies in the editing history of your pictures.
This is fine if you don't need to open your edits again in the future, but maybe not if you manage an estate.

After backing up your `~/.config/darktable` directory and the sidecar .XMP files of any pictures you intend to open
with the master branch, you may obtain the source as follows:

```bash
git clone --recurse-submodules --depth 1 https://github.com/darktable-org/darktable.git
cd darktable
```

See below (in "Using") how to start a test install of the unstable version without damaging your regular stable install and files.

#### Latest stable release

3.8.1

The darktable project releases two major versions every year, in mid-Summer and at Christmas, tagged with even numbers (e.g. 3.0, 3.2, 3.4, 3.6).
Minor revisions are tagged with a third digit (e.g. 3.0.1, 3.0.2) and mostly provide bug fixes and camera support.
You may want to compile these stable releases yourself to get better performance for your particular computer:

```bash
git clone --recurse-submodules --depth 1 https://github.com/darktable-org/darktable.git
cd darktable
git fetch --tags
git checkout tags/release-3.8.1
```

### Get submodules

Note that [LibXCF](https://github.com/houz/libxcf.git), [OpenCL](https://github.com/KhronosGroup/OpenCL-Headers.git), [rawspeed](https://github.com/darktable-org/rawspeed), [whereami](https://github.com/gpakosz/whereami) and [LibRaw](https://github.com/LibRaw/LibRaw) are tracked via git submodules, so after checking-out darktable, you need to update/checkout the submodules too:

```bash
git submodule update --init
```

### Compile

#### Easy way

WARNING: If you have previously built darktable, don't forget to first completely remove (`rm -R`) the `build`
and `/opt/darktable` directories to avoid conflicting files from different versions. Many weird behaviours and transient
bugs have been reported that can be traced to the build cache not properly invalidating the changed dependencies, so
the safest way is to completely remove previously built binaries and start again from scratch.

darktable provides a shell script that automatically takes care of building on Linux and macOS for classic cases in a single command.

```bash
./build.sh --prefix /opt/darktable --build-type Release --install --sudo
```

If you want to install a test version alongside your regular/stable version, change the install prefix:

```bash
./build.sh --prefix /opt/darktable-test --build-type Release --install --sudo
```

This builds the software for your architecture only, with:

* `-O3` optimization level,
* SSE/AVX support if detected,
* OpenMP support (multi-threading and vectorization) if detected,
* OpenCL support (GPU offloading) if detected,
* Lua scripting support if detected.

If you want to have dartkable displayed along your other applications, you only need to add a symbolic link:

```bash
ln -s /opt/darktable/share/applications/darktable.desktop /usr/share/applications/darktable.desktop
```

Now, your custom-built darktable is ready to be used just like any pre-packaged software.

#### Manual way

Alternatively, you can use a manual build to pass custom arguments.

##### Linux/macOS

```bash
mkdir build/
cd build/
cmake -DCMAKE_INSTALL_PREFIX=/opt/darktable/ ..
make
sudo make install
```

##### Windows

See https://github.com/darktable-org/darktable/blob/master/packaging/windows/BUILD.md

### Using

#### Test/unstable version

To use a test version of darktable without damaging your regular/stable version's files and database, start darktable in a terminal with:

```bash
/opt/darktable-test/bin/darktable --configdir "~/.config/darktable-test"
```

and ensure that you set the option "write sidecar file for each image" to "never" in preferences -> storage -> XMP. This way,
your regular/stable version will save its configuration files in `~/.config/darktable`, as usual,
the test/unstable one will save in `~/.config/darktable-test`, and the two versions will not produce database conflicts.

#### Regular/stable version

Simply launch it from your desktop application menu or, from a terminal, run `darktable` or `/opt/darktable/bin/darktable`. If the installation did not create a launcher in your applications menu, run:

```bash
sudo ln -s /opt/darktable/share/applications/darktable.desktop /usr/share/applications/darktable.desktop
```

You may find darktable configuration files in `~/.config/darktable`.
If you experience crashes at startup, try launching darktable without OpenCL, from a terminal, with `darktable --conf opencl=FALSE`.

### Further reading

There is a comprehensive list of build instructions for [Ubuntu/Debian related distributions](https://github.com/darktable-org/darktable/wiki/Build-instructions-for-Ubuntu) or for [Fedora and related distributions](https://github.com/darktable-org/darktable/wiki/Build-Instructions-for-Fedora). These build instructions can be easily adapted to many other Linux distributions.


Contributing
------------

There are many ways you can contribute to the darktable project:

* Write a blog about darktable
* Create a tutorial for darktable
* Help expand the [user wiki](https://github.com/darktable-org/darktable/wiki) or [user manual](https://github.com/darktable-org/dtdocs)
* Answer questions on the [user mailing list](https://www.mail-archive.com/darktable-user@lists.darktable.org/) or the [pixls.us forums](https://discuss.pixls.us)
* Share your ideas on the [developer mailing list](https://www.mail-archive.com/darktable-dev@lists.darktable.org/)
* Test [releases](https://www.darktable.org/install/)
* Review [pull requests](https://github.com/darktable-org/darktable/pulls)
* Start [hacking on darktable](https://www.darktable.org/development/) and see [developer's guide](https://github.com/darktable-org/darktable/wiki/Developer's-guide)


FAQ
---

### Why is my camera not detected when plugged-in ?

Check that you have the latest [gphoto2 library](http://www.gphoto.org/ "gphoto2 homepage") installed in order to support the newest cameras.

### Why is my lens not detected/corrected in darkroom ?

Lens correction profiles are provided by Lensfun, which has 2 parts: a program and a database.
Most Linux distributions provide a recent enough version of the program,
but provide an outdated version of the database. If
[Lensfun](https://lensfun.github.io/) is correctly installed, then update its database in a terminal by running:

```bash
lensfun-update-data
```

or alternatively

```bash
/usr/bin/g-lensfun-update-data
```

### Why do the thumbnails in the lighttable view look different to the preview in the darkroom view ?

For RAW files that have never been edited in darktable (when you have just imported them), the lighttable view, by default, shows
the JPEG preview placed into the RAW file by your camera. Loading this JPEG file is faster and makes the
lighttable view more responsive when importing large collections of images.

However, this JPEG thumbnail is processed by the firmware of the camera, with proprietary algorithms,
and colors, sharpness and contrast that might not look the same as
darktable processing (which is what you see when opening the image in the darkroom view).
Camera manufacturers don't publish details of the pixel processing they perform in their firmware
so their look is not exactly or easily reproducible by other software.

However, once RAW images have been edited in darktable,
the lighttable thumbnail should exactly match the darkroom preview, as they are processed in the same way.

If you never want to see the embedded JPEG thumbnail in the lighttable view, for RAW files, you should set the
option "use raw file instead of embedded JPEG from size" to "never" in preferences -> lighttable.

Wiki
----

* [GitHub wiki](https://github.com/darktable-org/darktable/wiki "github wiki")
* [Developer wiki](https://github.com/darktable-org/darktable/wiki/Developer's-guide "darktable developer wiki")


Mailing lists
-------------

* Users [[subscribe](mailto:darktable-user+subscribe@lists.darktable.org) | [archive](https://www.mail-archive.com/darktable-user@lists.darktable.org/)]
* Developer [[subscribe](mailto:darktable-dev+subscribe@lists.darktable.org) | [archive](https://www.mail-archive.com/darktable-dev@lists.darktable.org/)]
* CI (read-only, high traffic!) [[subscribe](mailto:darktable-ci+subscribe@lists.darktable.org) | [archive](https://www.mail-archive.com/darktable-ci@lists.darktable.org/)]
