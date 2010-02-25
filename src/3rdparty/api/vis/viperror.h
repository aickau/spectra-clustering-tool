/////////////////////////////////////////////
//
// viperror.h
//
#if !defined( _VIPERROR_H_KHADFGKSJDGKJSHGOWEZRTWUIEZT923526592705715OTHJLSH184581568987646XV646G46SG78S6G7 )
#define _VIPERROR_H_KHADFGKSJDGKJSHGOWEZRTWUIEZT923526592705715OTHJLSH184581568987646XV646G46SG78S6G7

#define VIPTHROW( errorClassName , errorCode ) throw errorClassName( (errorCode) , __LINE__ , __FILE__ , 0 );
#define VIPTHROW_MSG( errorClassName , errorCode , msg ) throw errorClassName( (errorCode) , __LINE__ , __FILE__ , (msg) );

/** Basis-Fehlerklasse.
*/
class CVIPError
{
private:
	
	/** Fehlercode, welcher den Fehler beschreibt.
	*/
	HRESULT	m_errorCode;
	
	/** Zeilennummer der Stelle, an der das CHECK-Makro stand, innerhalb dessen die fehlersignalisierende
	Schnittstelle aufgerufen wurde.
	@see CHECK
	*/
	int		m_line;
	
	/** Name der Datei, in der das CHECK-Makro stand, innerhalb dessen die fehlersignalisierende
	Schnittstelle aufgerufen wurde.
	Der Name besteht aus Datei- und Pfadnamen.
	@see CHECK
	*/
	char*	m_file;
	
	/** Klartextfehlermeldung.
	*/
	char*	m_message;
	
public:
	
	CVIPError() : m_errorCode( S_OK ) , m_line( 0 ) , m_file( 0 ) , m_message( 0 ) {};
	CVIPError( HRESULT errorCode , int line , const char* file , const char* message ) : m_errorCode( errorCode ) , m_line( line ) , m_file( _strdup( file ) ) , m_message( _strdup( message ) ) {};
	CVIPError( const CVIPError& src ) : m_errorCode( src.m_errorCode ) , m_line( src.m_line ) , m_file( _strdup( src.m_file ) ) , m_message( _strdup( src.m_message ) ) {};
	~CVIPError()
	{
		free( m_file );
		free( m_message );
	}
	
	
	CVIPError& operator = ( const CVIPError& src )
	{
		m_errorCode = src.m_errorCode;
		m_line = src.m_line;
		
		free( m_file );
		m_file = _strdup( src.m_file );
		
		free( m_message );
		m_message = _strdup( src.m_message );
		
		return *this;
	};
	
	
	/** Liefert den Fehlercode zurück.
	*/
	HRESULT	GetErrorCode() const
	{
		return m_errorCode;
	};
	
	
	/** Liefert die Zeilennummer der Stelle, an der das CHECK-Makro stand, 
	innerhalb dessen die fehlersignalisierende Schnittstelle aufgerufen wurde, zurück.
	@see CHECK
	*/
	int		GetLine() const
	{
		return m_line;
	};
	
	
	/** Liefert den Namen der Datei, in der das CHECK-Makro stand, innerhalb dessen die 
	fehlersignalisierende Schnittstelle aufgerufen wurde, zurück.
	Der Name besteht aus Datei- und Pfadnamen.
	@see CHECK
	*/
	const char*	GetFile() const
	{
		return m_file;
	};
	
	
	/** Liefert, sofern vorhanden, eine Klartextfehlermeldung zurück.
	@return Zeiger auf Klartextfehlermeldung oder NULL.
	*/
	const char*	GetErrorMessage() const
	{
		return m_message;
	};
	
	
};

#endif // _VIPERROR_H_KHADFGKSJDGKJSHGOWEZRTWUIEZT923526592705715OTHJLSH184581568987646XV646G46SG78S6G7