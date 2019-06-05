
namespace sio {
  
  inline version::version_type version::encode_version( major_type major, minor_type minor ) noexcept {
    return (((major) << 16) + (minor)) ;
  }

  //--------------------------------------------------------------------------

  inline version::minor_type version::minor_version( version_type version ) noexcept {
    return ((version) & 0x0000ffff) ;
  }

  //--------------------------------------------------------------------------

  inline version::major_type version::major_version( version_type version ) noexcept {
    return (((version) & 0xffff0000) >> 16) ;
  }
  
}