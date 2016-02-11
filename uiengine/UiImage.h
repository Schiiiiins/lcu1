#pragma once
#include "UiElement.h"
#include <vector>

class CachedImage;

////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>	User interface image element. Represents a PNG image on a UI page. </summary>
///
/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
////////////////////////////////////////////////////////////////////////////////////////////////////

class UiImage: public UiElement
{
public:

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Default constructor. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	UiImage(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Finaliser. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	~UiImage(void);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Implementation of <see cref="UiElement::InitfromXml" />.
	/// 			Initializes this object from an XML description. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="rootNode">	[in] The XML node containing the description for this object. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool InitFromXml( XMLNode& rootNode );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Implementation of <see cref="UiElement::Render" />. Renders this object on 
	/// 			the directFB surface. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual void Render();

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Implementation of <see cref="UiElement::SetProperty" />. Called from the TCL 
	/// 			<c>ui set</c> command. 
	/// 			Supported properties (in addition to the <c>UiElement</c> properties:
	/// 			- <c>int index</c>: If multiple PNG images are assiciated with this element, index of image to show.
	/// 			</summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="id">   	The property identifier. </param>
	/// <param name="value">	The property value. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool SetProperty( const std::string& id, const std::string& value );

	////////////////////////////////////////////////////////////////////////////////////////////////////
	/// <summary>	Implementation of <see cref="UiElement::GetProperty" />.
	/// 			<see cref="SetProperty" /> for a list of properties. </summary>
	///
	/// <remarks>	Lars Reemts, 03.05.2011. </remarks>
	///
	/// <param name="id">   	The identifier. </param>
	/// <param name="value">	[out] The value. </param>
	///
	/// <returns>	true if it succeeds, false if it fails. </returns>
	////////////////////////////////////////////////////////////////////////////////////////////////////

	virtual bool GetProperty( const std::string& id, std::string* value );
private:
	/// <summary> The list of associated images </summary>
	std::vector<CachedImage*> _cachedImages;
	/// <summary> Zero-based index of the image currently shown </summary>
	unsigned int _imageIndex;
};

////////////////////////////////////////////////////////////////////////////////////////////////////
// End of UiImage.h
////////////////////////////////////////////////////////////////////////////////////////////////////
