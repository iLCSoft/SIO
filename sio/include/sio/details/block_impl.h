
namespace sio {
  
  inline block::block( const std::string &nam, sio::version_type vers ) :
    _version(vers),
    _name(nam) {
    /* nop */
  }
  
  //--------------------------------------------------------------------------
  
  inline const std::string &block::name() const noexcept {
    return _name ;
  }

  //--------------------------------------------------------------------------

  inline sio::version_type block::version() const noexcept {
    return _version ;
  }
  
}