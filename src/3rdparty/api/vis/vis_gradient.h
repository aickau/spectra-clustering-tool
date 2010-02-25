#ifndef _VIS_GRADIENT_H
#define _VIS_GRADIENT_H

#include <afxtempl.h>

#include "vis_dllexport.h"

/** Klasse zur Verwaltung der einzelnen Farbmarker im Farbgradienten.
 */
class CVisPeg
{
public:
	CVisPeg();
	CVisPeg(const CVisPeg &src);
	CVisPeg& operator = (const CVisPeg &src);

	/** liefert für jeden Marker eine eindeutige ID.
	 * @return eindeutige,fortlaufende ID, beginnend mit 1.
	 */
	const UINT	GetID() {return m_ID;}

	/** setzt die Position des Farbmarkers.
	 * @param position Position des Farbmarkers [0..1], 0=0%, 1=100%. 
	 */
	void		SetPosition(float position) { m_Position = (position < 0.0f) ? 0.0f : (position > 1.0f) ? 1.0f : position;	}
	
	/** Hole Position.
	 * @return Position im Bereich [0..1].
	 */
	float		GetPosition() { return m_Position; }
	void		SetColor(COLORREF color) { m_Color = color; }
	COLORREF	GetColor() { return m_Color; }


protected:

	COLORREF	m_Color;
	float		m_Position;
	UINT		m_ID;
	static UINT m_IDCount;
};


/** Klasse zur Verwaltung und Erstellung von Farbgradienten. 
 *Gradienten bestehen aus mehreren Farbmarkern, sowie einen Anfangs- und Endmarker (Peg).
 */
class CVisGradient
{
public:
	VIS_DLLEXPORT CVisGradient();
	VIS_DLLEXPORT CVisGradient(CVisGradient &gradient);
	VIS_DLLEXPORT virtual ~CVisGradient();

	VIS_DLLEXPORT CVisGradient& operator =(const CVisGradient &src);
	
	/** @return Liefert Anzahl der Pegs in m_PegArray zurück.
	 */
	int				GetPegCount() { return static_cast<int>(m_PegArray.GetSize()); }

	/** Hole Peg.
	 * @param index Feldposition in m_PegArray.
	 * @return Zeiger zu CVisPeg oder NULL wenn es sich um einen Start- oder End handelt oder wenn index ausserhalb des Bereiches liegt.
	 */
	CVisPeg			*GetPeg(int index);

	
	int				SetPeg(int index, float position);

	/** Setze Peg Parameter. Nicht für Start- und EndPeg, da deren Positionen fest bei 0 bzw. 1 sind.
	 * @param index Feldposition in m_PegArray.
	 * @param color neuer Farbwert.
	 * @param position neue Position im Bereich [0..1].
	 * @return neuer Index in m_PegArray, nach Sortierung, oder -1 wenn Index ungültig.
	 */
	int				SetPeg(int index, COLORREF color, float position);


	/** Hinzufügen eines neuen Pegs.
	 * @param color Farbwert.
	 * @param position Position im Bereich [0..1].
	 * @return Index in m_PegArray, nach Sortierung.
	 */
	int				AddPeg(COLORREF color, float position);
	
	/** Löschen eines Pegs. Start- und Endpegs werden nicht gelöscht.
	 * @param index Feldposition in m_PegArray.
	 */
	void			RemovePeg(int index);

	/** Liefert Index zum nächsten Peg. 
	* @param position im Bereich [0..1].
	* @return Feldposition in m_PegArray, oder -1 wenn position ausserhalb des Bereiches liegt.
	*/
	int				IndexFromPosition(float position);

	/** Setze Start-Farbe des Gradienten.
	*/
	void			SetStartPegColor(const COLORREF color);

	/** Hole Start-Farbe des Gradienten.
	*/
	COLORREF		GetStartPegColor();

	/** Setze End-Farbe des Gradienten.
	*/
	void			SetEndPegColor(const COLORREF color);
	
	/** Hole End-Farbe des Gradienten.
	*/
	COLORREF		GetEndPegColor();

	/** Hole Quantisierungsstufe des Gradienten. 
	 * @return Quantisierungsstufe. Falls < 0 -> keine Quantisierung.
	*/
	int				GetQuantization() { return m_Quantization; }

	/** Setze Quantisierungsstufe des Gradienten. 
	 * @param steps Anzahl der Stufen. Wenn keine Quantisierung erfolgen soll: -1.
	*/
	void			SetQuantization(const int steps) { m_Quantization = steps; }

	/** Berechne Palette mit Farbgradienten.
	 * @param lpPal zu füllendes Feld mit Farbwerten.
	 * @param iEntryCount Feldgröße.
  	 */
	void			MakeEntries(RGBTRIPLE *lpPal, int iEntryCount);
	
protected:
	/** sortiert m_PegArray, aufsteigend nach CVisPeg::Position.
	*/
	void			SortPegs();
	
private:
	/** Wandlung von PegID -> PegIndex. Indezies sind nach einfügen neuer Pegs nicht mehr aktuell.
	 * @param id valide Id von CVisPeg::GetID()
	 * @return Index zu m_PegArray oder -1, falls Peg nicht im Feld enthalten ist.
	*/
	int				IndexFromId(UINT id);

	/** lineare Interpolation zw. 2 Farbwerten/Pegs.
	 * @param first 1. Farbwert.
	 * @param second 2. Farbwert.
	 * @param position aktuelle Position im Bereich [start..end]
	 * @param start Position des 1. Farbwertes im Bereich [0..1].
	 * @param end Position des 2. Farbwertes im Bereich [0..1].
	 * @return aktueller, interpolierter Farbwert.
	*/
	COLORREF		InterpolateLinear(COLORREF first, COLORREF second, float position, float start, float end);

protected:
	/** Array zur Verwaltung der Pegs. die Pegs sind nach Position aufsteigend sortiert.
	*/
	CArray <CVisPeg, CVisPeg&> m_PegArray;
	/** Quantisierungsstufe des Gradienten. Wenn m_Quantization < 0 -> keine Quantisierung.
	*/
	int				m_Quantization;
	/** Start- und EndPeg ID's.
	*/
	UINT			m_StartPegID, m_EndPegID;
};

#endif