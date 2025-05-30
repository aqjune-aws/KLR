/-
Copyright (c) 2024 Amazon.com, Inc. or its affiliates. All Rights Reserved.
Released under Apache 2.0 license as described in the file LICENSE.
Authors: Paul Govereau, Sean McLaughlin
-/

namespace KLR.Util.Archive

structure ArchiveEntry where
  filename : String
  content : ByteArray
  deriving Inhabited

instance : Repr ArchiveEntry where
  reprPrec e _ := repr e.filename

@[extern "lean_archive_create_tar"]
opaque createTar (entries : @&List ArchiveEntry) : ByteArray

@[extern "lean_archive_extract_tar"]
opaque extractTar (bytes : @&ByteArray) : List ArchiveEntry

end KLR.Util.Archive
